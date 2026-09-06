//

// Base:
#include "RTS_Character.h"

// UE:
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpectatorPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Interaction:
#include "RTS_PlayerController.h"
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ARTS_Character::ARTS_Character()
{
    // Set this pawn to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true; // Принудительно
    //SetActorTickInterval(0.1f); // 10 раз/сек.

    // Управление по умолчанию из родительского класса не подходит
    bAddDefaultMovementBindings = false;

    // Включить Управление камерой через Контроллер Игрока
    bUseControllerRotationYaw = true;
    //-------------------------------------------


    /* ---   Components   --- */

    /* Корневой компонент: Сферическая коллизия */
    GetCollisionComponent()->SetCollisionProfileName(ProfileName_Spectator);

    /* Держатель камеры */
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = CameraDistance_Setpoint;
    SpringArm->bDoCollisionTest = false;
    SpringArm->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));

    /* Камера */
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    //-------------------------------------------


    /* ---   Non-scene Components   --- */

    /* Компонент перемещения "Наблюдателя", который инициализируется в родительском классе 'ASpectatorPawn' */
    if (USpectatorPawnMovement* lSPM = Cast<USpectatorPawnMovement>(GetMovementComponent()))
    {
        lSPM->MaxSpeed = 12'000.f;
        lSPM->Acceleration = 40'000.f;
        lSPM->Deceleration = 80'000.f;
    }
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void ARTS_Character::BeginPlay()
{
    Super::BeginPlay();

    /* Учёт сетевых нюансов (использовать, при необходимости):
    Старт инициализации для Игрока-Клиента (`Listen Server`)
    @note   Контроллер не валиден в момент запуска Отложенного(!) матча
            (если есть период ожидания начала матча при использовании кода класса 'AGameMode') */
            //if (!HasAuthority() && IsLocallyControlled())
    {
        InitScreenEdgeControl();
        InitHeightControl();
    }
}

void ARTS_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ScreenEdgeControl(DeltaTime);
    CameraRangeControl(DeltaTime);
    LocationHeightControl(DeltaTime);
}

void ARTS_Character::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    /* Учёт сетевых нюансов (использовать, при необходимости):
    Старт инициализации для Игрока-Сервера (`Listen Server`)
    @note   'PossessedBy(*)' не вызывается на стороне Клиента */
    //if (IsLocallyControlled())
    //{
    //    InitScreenEdgeControl();
    //}
}

void ARTS_Character::UnPossessed()
{
    Super::UnPossessed();

    bScreenEdgeControl = false;
    FViewport::ViewportResizedEvent.RemoveAll(this);
}
//--------------------------------------------------------------------------------------



/* ---   Inputs   --- */

#define BindAxisGroups(AxisGroups, Function) \
{ \
    if (AxisGroups != NAME_None) \
        PlayerInputComponent->BindAxis(AxisGroups, this, &ARTS_Character::Function); \
}

void ARTS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    //Super::SetupPlayerInputComponent(PlayerInputComponent);
    // @note    Управление по умолчанию из родительского класса не подходит


    /* ===   Actions   === */

    /* ---   Actions | Test   --- */

    //if (ActionGroups_Test != NAME_None)
    //{
    //    PlayerInputComponent->BindAction(ActionGroups_Test, IE_Pressed, this, &ACharacter::Test);
    //    PlayerInputComponent->BindAction(ActionGroups_Test, IE_Released, this, &ACharacter::StopTest);
    //}
    //-------------------------------------------
    //===========================================


    /* ===   Axis   === */

    /* ---   Axis | Movement   --- */

    BindAxisGroups(AxisGroups_MoveForward, MoveForward);
    BindAxisGroups(AxisGroups_MoveRight, MoveRight);
    //-------------------------------------------


    /* ---   Inputs | Camera   --- */

    BindAxisGroups(AxisGroups_Turn, TurnAtRate);
    BindAxisGroups(AxisGroups_LookUp, LookUpAtRate);
    BindAxisGroups(AxisGroups_CameraDistance, CameraRange);
    //-------------------------------------------
    //===========================================


    /* ===   For EDITOR only   === */

#if WITH_EDITOR

    /* ---   Inputs   --- */

    CheckAxisGroups({
        AxisGroups_MoveForward,
        AxisGroups_MoveRight,
        AxisGroups_Turn,
        AxisGroups_LookUp,
        AxisGroups_CameraDistance });
    //-------------------------------------------

#endif // WITH_EDITOR
    //===========================================
}
//--------------------------------------------------------------------------------------



/* ---   Inputs | Movement   --- */

/* Макрос: Расчёт Скорости Перемещения */
#define MOVSPEED_CALCULATION (CameraDistance_Setpoint + 500) * MovementSpeed * GetDeltaSeconds()

void ARTS_Character::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value * MOVSPEED_CALCULATION);
        UnPauseHeightControl();
    }
}

void ARTS_Character::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value * MOVSPEED_CALCULATION);
        UnPauseHeightControl();
    }
}

void ARTS_Character::MoveUp_World(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(FVector::UpVector, Value * MOVSPEED_CALCULATION);
    }
}
//--------------------------------------------------------------------------------------



/* ---   Inputs | Movement | Screen Edge Control   --- */

FORCEINLINE void ARTS_Character::ScreenEdgeControl(float DeltaTime)
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'
    if (bScreenEdgeControl)
    {
        if (CurrentViewport)
        {
            // Получение текущей позиции курсора
            FIntPoint lPos;
            CurrentViewport->GetMousePos(lPos);

            if (lPos != FIntPoint::NoneValue)
            {
                // Вычисленная скорость перемещения
                FVector Speed = FVector::ZeroVector;

                if (lPos.X < SensitiveZone.X)
                {
                    Speed.Y = (float(lPos.X) / SensitiveZone.X) - 1;
                }
                else if (CurrentSize.X - lPos.X < SensitiveZone.X)
                {
                    Speed.Y = (float(lPos.X - CurrentSize.X) / SensitiveZone.X) + 1;
                }

                if (lPos.Y < SensitiveZone.Y)
                {
                    Speed.X = (float(-lPos.Y) / SensitiveZone.Y) + 1;
                }
                else if (CurrentSize.Y - lPos.Y < SensitiveZone.Y)
                {
                    Speed.X = (float(CurrentSize.Y - lPos.Y) / SensitiveZone.Y) - 1;
                }

                if (Speed.X || Speed.Y)
                {
                    AddMovementInput(GetTransform().TransformVectorNoScale(Speed),
                        (CameraDistance_Setpoint + 500) * MovementSpeed * DeltaTime);

                    UnPauseHeightControl();
                }
            }
        }
    }
}

FORCEINLINE void ARTS_Character::InitScreenEdgeControl()
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'

    if (ARTS_PlayerController* lPC = GetController<ARTS_PlayerController>())
    {
        OnViewportResized(lPC->GetCurrentViewport(), 0);
        FViewport::ViewportResizedEvent.AddUObject(this, &ARTS_Character::OnViewportResized);
    }
    else
    {
        M_Error("'Controller' is NOT 'ARTS_PlayerController'");
    }
}

void ARTS_Character::OnViewportResized(FViewport* Viewport, uint32 Params)
{
    CurrentViewport = Viewport;

    if (CurrentViewport)
    {
        // Получение размера экрана (может изменяться в процессе игры)
        CurrentSize = CurrentViewport->GetSizeXY();

        /* Получение зоны чувствительности экрана
        @note   Необходим для уменьшения количества операций обязательного умножения */
        SensitiveZone = FVector2D(
            CurrentSize.X * SensitiveZonePercentage.X,
            CurrentSize.Y * SensitiveZonePercentage.Y
        );

        bScreenEdgeControl = true;
    }
}
//--------------------------------------------------------------------------------------



/* ---   Inputs | Movement | Height Control   --- */

void ARTS_Character::LocationHeightControl(float DeltaTime)
{
    if (bHeightControl)
    {
        float lCurrZ = GetActorLocation().Z;

        if (!FMath::IsNearlyEqual(lCurrZ, GetTargetLocationHeight(), 0.1f))
        {
            MoveUp_World((GetTargetLocationHeight() - lCurrZ) * HeightControl_InterpSpeed * DeltaTime);
        }
    }
}

void ARTS_Character::UpdateTargetLocationHeight()
{
    FVector lCurrLocation = GetActorLocation();

    UKismetSystemLibrary::SphereTraceSingle(
        GetWorld(),
        FVector(lCurrLocation.X, lCurrLocation.Y, lCurrLocation.Z + CameraDistance_Range.Y),
        FVector(lCurrLocation.X, lCurrLocation.Y, lCurrLocation.Z - CameraDistance_Range.Y),
        CameraDistance_Setpoint,
        HeightControl_TraceType,
        false,
        TArray<AActor*>(),
        EDrawDebugTrace::None,
        Hit_HeightControl,
        true);

    if (FMath::IsNearlyEqual(lCurrLocation.Z, GetTargetLocationHeight(), 0.1f))
    {
        PauseHeightControl();
    }
    else
    {
        bHeightControl = true;
    }
}

void ARTS_Character::InitHeightControl()
{
    GetWorldTimerManager().SetTimer(
        Timer_HeightControl,
        this,
        &ARTS_Character::UpdateTargetLocationHeight,
        HeightControl_UpdateDelay,
        true);
}
//--------------------------------------------------------------------------------------



/* ---   Inputs | Camera   --- */

void ARTS_Character::TurnAtRate(float Rate)
{
    // Обход логики из родительского класса 'ASpectatorPawn'
    Super::Super::TurnAtRate(Rate);
}

void ARTS_Character::LookUpAtRate(float Rate)
{
    if (Rate != 0)
    {
        FRotator CurRot = SpringArm->GetRelativeRotation();

        CurRot.Pitch = FMath::Clamp(
            CurRot.Pitch + (Rate * BaseLookUpRate * GetDeltaSeconds() * CustomTimeDilation),
            CameraRotation_LookUpRange.X,
            CameraRotation_LookUpRange.Y);

        SpringArm->SetRelativeRotation(CurRot);
    }
}

void ARTS_Character::CameraRange(float Value)
{
    if (Value != 0)
    {
        float lSetpoint = FMath::Clamp(
            CameraDistance_Setpoint + (Value * CameraDistance_SetpointChangeStep * CameraDistance_Setpoint),
            CameraDistance_Range.X,
            CameraDistance_Range.Y);

        if (lSetpoint != CameraDistance_Setpoint)
        {
            CameraDistance_Setpoint = lSetpoint;
            UnPauseHeightControl();
        }
    }
}

FORCEINLINE void ARTS_Character::CameraRangeControl(float DeltaTime)
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'
    if (CameraDistance_Setpoint != SpringArm->TargetArmLength)
    {
        float lDelta = CameraDistance_Setpoint - SpringArm->TargetArmLength;
        SpringArm->TargetArmLength += lDelta * DeltaTime * CameraDistance_ChangeSpeed;

        if (FMath::IsNearlyEqual(CameraDistance_Setpoint, SpringArm->TargetArmLength, 1.f))
        {
            SpringArm->TargetArmLength = CameraDistance_Setpoint;
        }
    }
}
//--------------------------------------------------------------------------------------



/* ===   For EDITOR only   === */

#if WITH_EDITOR

/* ---   Debugs   --- */

#define CheckPropertyName(Param) \
{ \
    if(PropertyName == GET_MEMBER_NAME_CHECKED(ARTS_Character, Param)) \
        CheckAxisGroups({ Param }); \
}

void ARTS_Character::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.Property)
    {
        // Здесь можно написать логику проверки изменённого свойства.
        FName PropertyName = PropertyChangedEvent.Property->GetFName();

        CheckPropertyName(AxisGroups_MoveForward);
        CheckPropertyName(AxisGroups_MoveRight);
        CheckPropertyName(AxisGroups_Turn);
        CheckPropertyName(AxisGroups_LookUp);
        CheckPropertyName(AxisGroups_CameraDistance);
    }
}
//--------------------------------------------------------------------------------------

#endif // WITH_EDITOR
//======================================================================================
