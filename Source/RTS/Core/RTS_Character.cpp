//

// Base:
#include "RTS_Character.h"

// Macros:
#include "RTS/Tools/GlobalMacros.h"

// UE:
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Interaction:
#include "RTS_PlayerController.h"
//--------------------------------------------------------------------------------------



/* ---   Macros   --- */

// Макрос: Расчёт Скорости Перемещения
#define MOVSPEED_CALCULATION (CameraHeight_Setpoint + 500) * MovementSpeed * 0.00001f
//--------------------------------------------------------------------------------------



/* ---   Constructors   --- */

ARTS_Character::ARTS_Character()
{
    // Set this pawn to call Tick() every frame.
    // You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true; // Принудительно
    //SetActorTickInterval(0.1f); // 10 раз/сек.

    bGenerateOverlapEventsDuringLevelStreaming = false;
    //-------------------------------------------


    /* ---   Components   --- */

    /* Корневой компонент */
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    /* Держатель камеры */
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = CameraHeight_Setpoint;
    SpringArm->bDoCollisionTest = false;
    SpringArm->SetRelativeRotation(FRotator(-70.f, 0.f, 0.f));

    /* Камера */
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    //-------------------------------------------


    /* ---   Non-scene Components   --- */

    /* Компонент плавного Перемещения */
    FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement"));
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

//void ARTS_Character::BeginPlay()
//{
//    Super::BeginPlay();
//}

void ARTS_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ScreenEdgeControl();
    CameraHeightControl(DeltaTime);
}

void ARTS_Character::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (Cast<ARTS_PlayerController>(NewController))
    {
        bScreenEdgeControl = true;
    }
    else
    {
        RTS_Error("'NewController' is NOT 'ARTS_PlayerController'");
    }
}

void ARTS_Character::UnPossessed()
{
    Super::UnPossessed();
    
    bScreenEdgeControl = false;
    CurrentViewport = nullptr;
}
//--------------------------------------------------------------------------------------



/* ---   Inputs   --- */

void ARTS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);


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

    if (AxisGroups_MoveForward != NAME_None)
        PlayerInputComponent->BindAxis(AxisGroups_MoveForward, this, &ARTS_Character::MoveForward);

    if (AxisGroups_MoveRight != NAME_None)
        PlayerInputComponent->BindAxis(AxisGroups_MoveRight, this, &ARTS_Character::MoveRight);
    //-------------------------------------------


    /* ---   Inputs | Camera   --- */

    if (AxisGroups_CameraHeight != NAME_None)
        PlayerInputComponent->BindAxis(AxisGroups_CameraHeight, this, &ARTS_Character::MoveCamera);
    //-------------------------------------------
    //===========================================


    /* ===   For EDITOR only   === */

#if WITH_EDITOR

    /* ---   Inputs   --- */

    CheckInputsGroups();
    //-------------------------------------------

#endif // WITH_EDITOR
    //===========================================
}
//--------------------------------------------------------------------------------------



/* ---   Inputs | Movement   --- */

void ARTS_Character::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value * MOVSPEED_CALCULATION);
    }
}

void ARTS_Character::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value * MOVSPEED_CALCULATION);
    }
}

FORCEINLINE void ARTS_Character::ScreenEdgeControl()
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'
    if (bScreenEdgeControl)
    {
        if (CurrentViewport)
        {
            // Получение размера экрана (может изменяться в процессе игры)
            FIntPoint lSize = CurrentViewport->GetSizeXY();

            /* Получение зоны чувствительности экрана
            @note   Необходим для уменьшения количества операций обязательного умножения */
            FIntPoint lSizeZone = FIntPoint(
                lSize.X * SensitiveZonePercentage.X,
                lSize.Y * SensitiveZonePercentage.Y
            );

            // Получение текущей позиции курсора
            FIntPoint lPos;
            CurrentViewport->GetMousePos(lPos);

            // Вычисленная скорость перемещения
            FVector Speed = FVector(0);

            if (lPos.X < lSizeZone.X)
            {
                Speed.Y = (float(-lPos.X) / float(lSizeZone.X)) + 1;
            }
            else if (lSize.X - lPos.X < lSizeZone.X)
            {
                Speed.Y = (float(lSize.X - lPos.X) / float(lSizeZone.X)) - 1;
            }

            if (lPos.Y < lSizeZone.Y)
            {
                Speed.X = (float(lPos.Y) / float(lSizeZone.Y)) - 1;
            }
            else if (lSize.Y - lPos.Y < lSizeZone.Y)
            {
                Speed.X = (float(lPos.Y - lSize.Y) / float(lSizeZone.Y)) + 1;
            }

            if (Speed.X || Speed.Y)
            {
                AddMovementInput(Speed, MOVSPEED_CALCULATION);
            }
        }
        else
        {
            if (ARTS_PlayerController* lPC = Cast<ARTS_PlayerController>(GetController()))
            {
                CurrentViewport = lPC->GetCurrentViewport();
            }
        }
    }
}
//--------------------------------------------------------------------------------------



/* ---   Inputs | Camera   --- */

void ARTS_Character::MoveCamera(float Value)
{
    if (Value != 0)
    {
        CameraHeight_Setpoint = FMath::Clamp(
            CameraHeight_Setpoint + (Value * CameraHeight_SetpointChangeSpeed * CameraHeight_Setpoint),
            CameraHeight_Range.X,
            CameraHeight_Range.Y);
    }
}

FORCEINLINE void ARTS_Character::CameraHeightControl(float DeltaTime)
{
    // @note    'FORCEINLINE' действует в пределах данного '.cpp'
    if (CameraHeight_Setpoint != SpringArm->TargetArmLength)
    {
        float lDelta = CameraHeight_Setpoint - SpringArm->TargetArmLength;
        SpringArm->TargetArmLength += lDelta * DeltaTime * CameraHeight_ChangeSpeed;

        if (UKismetMathLibrary::NearlyEqual_FloatFloat(CameraHeight_Setpoint, SpringArm->TargetArmLength, 1.f))
        {
            SpringArm->TargetArmLength = CameraHeight_Setpoint;
        }
    }
}
//--------------------------------------------------------------------------------------



/* ===   For EDITOR only   === */

#if WITH_EDITOR

/* ---   Inputs   --- */

TArray<FName> ARTS_Character::GetActionGroupsNames()
{
    TArray<FName> ActionNames;

    UInputSettings::GetInputSettings()->GetActionNames(ActionNames);

    return ActionNames;
}

TArray<FName> ARTS_Character::GetAxisGroupsNames()
{
    TArray<FName> AxisNames;

    UInputSettings::GetInputSettings()->GetAxisNames(AxisNames);

    return AxisNames;
}

void ARTS_Character::CheckInputsGroups()
{
    //TArray<FName> lUsed_Actions = {
    //    ActionGroups_Test,
    //};

    //TArray<FName> lArray_ActionNames;
    //UInputSettings::GetInputSettings()->GetActionNames(lArray_ActionNames);

    //for (FName& Data : lUsed_Actions)
    //{
    //    if (Data == NAME_None)
    //    {
    //        RTS_LOG(Warning, "Not used at least one of the Actions ('%s')",
    //            *Data.ToString());
    //    }
    //    else if (lArray_ActionNames.Find(Data) == INDEX_NONE)
    //    {
    //        RTS_Error("'%s' is NOT an Action",
    //            *Data.ToString());
    //    }
    //}

    TArray<FName> lUsed_Axis = {
        AxisGroups_MoveForward,
        AxisGroups_MoveRight,
    };

    TArray<FName> lArray_AxisNames;
    UInputSettings::GetInputSettings()->GetAxisNames(lArray_AxisNames);

    for (FName& Data : lUsed_Axis)
    {
        if (Data == NAME_None)
        {
            RTS_LOG(Warning, "Not used at least one of the Axes ('%s')",
                *Data.ToString());
        }
        else if (lArray_AxisNames.Find(Data) == INDEX_NONE)
        {
            RTS_Error("'%s' is NOT an Axis",
                *Data.ToString());
        }
    }
}
//--------------------------------------------------------------------------------------

#endif // WITH_EDITOR
//======================================================================================
