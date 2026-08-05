//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/SpectatorPawn.h"

// Generated:
#include "RTS_Character.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class UCameraComponent;
class USpringArmComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_Character : public ASpectatorPawn
{
    GENERATED_BODY()

public:

    /* ---   Constructors   --- */

    // Sets default values for this character's properties
    ARTS_Character();
    //-------------------------------------------



    /* ---   Components   --- */

    /* Держатель камеры */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm = nullptr;

    /* Камера */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Вызывается при Запуске игры или при Спавне в уже запущенной игре
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Функция, вызываемая каждый кадр в этом Акторе, если не назначена другая частота */
    virtual void Tick(float DeltaSeconds) override;

    /** Вызывается при подключения Контроллера
    @note   Вызывается только на сервере (или в автономном режиме)
    @param  NewController - Контроллер, захвативший владение данным Игроком (Пешкой) */
    virtual void PossessedBy(AController* NewController) override;

    /** Вызывается при отключении Контроллера */
    virtual void UnPossessed() override;
    //-------------------------------------------



    /* ---   Inputs   --- */

    /** Вызывается для привязки к входным данным */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //-------------------------------------------



    /* ---   Inputs | Actions   --- */

    // Группа Действий для "Test"
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
    //    Category = "RTS Character|Inputs|Actions",
    //    meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetActionGroupsNames",
    //        DisplayName = "Test"))
    //FName ActionGroups_Test = NAME_None;
    //-------------------------------------------



    /* ---   Inputs | Axis   --- */

    /* Группа Осей: "Движение Вперёд" (вперёд-назад) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Character|Inputs|Axis",
        meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames",
            DisplayName = "Move Forward"))
    FName AxisGroups_MoveForward = NAME_None;

    /* Группа Осей: "Движение Вправо" (вправо-влево) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Character|Inputs|Axis",
        meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames",
            DisplayName = "Move Right"))
    FName AxisGroups_MoveRight = NAME_None;

    /* Группа Осей: "Движение Вверх" (вверх-вниз) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Character|Inputs|Axis",
        meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames",
            DisplayName = "Move Up"))
    FName AxisGroups_MoveUp = NAME_None;

    /* Группа Осей: Обзора Камерой по горизонтали (вправо-влево) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Character|Inputs|Axis",
        meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames",
            DisplayName = "Turn"))
    FName AxisGroups_Turn = NAME_None;

    /* Группа Осей: Обзора Камерой по вертикали (вверх-вниз) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Character|Inputs|Axis",
        meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames",
            DisplayName = "Look Up"))
    FName AxisGroups_LookUp = NAME_None;

    /* Группа Осей: Задание Дальности Камеры */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Character|Inputs|Axis",
        meta = (GetOptions = "GlobalUtilities.BlueprintGlobalFunctions.GetAxisGroupsNames",
            DisplayName = "Camera Distance"))
    FName AxisGroups_CameraDistance = NAME_None;
    //-------------------------------------------



    /* ---   Inputs | Movement   --- */

    /* Процент Зоны Чувствительности для управления через наведение мыши на Края Экрана */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Movement",
        meta = (AllowPreserveRatio, ClampMin = 0, UIMin = 0, ClampMax = 0.5f, UIMax = 0.5f))
    FVector2D SensitiveZonePercentage = FVector2D(0.1f);

    /* Скорость перемещения */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Movement",
        meta = (ClampMin = 0.001f, UIMin = 0.001f, ClampMax = 1.f, UIMax = 1.f))
    float MovementSpeed = 0.005f;

    //

    /** Установить контроль управления через наведение мыши на Край Экрана */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Character|Inputs|Movement")
    void SetScreenEdgeControl(bool Value)
    {
        bScreenEdgeControl = Value;
    };
    //-------------------------------------------



    /* ---   Inputs | Camera   --- */

    /* Диапазон Дальности Камеры */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Camera|Distance",
        meta = (DisplayName = "Range",
            ClampMin = 0, UIMin = 0, ClampMax = 50000, UIMax = 50000))
    FVector2D CameraDistance_Range = FVector2D(10.f, 5000.f);

    /* Скорость изменения дальности Камеры */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Camera|Distance",
        meta = (DisplayName = "Change Speed",
            ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 100, UIMax = 100))
    float CameraDistance_ChangeSpeed = 5.f;

    /* Заданная дальность камеры */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Camera|Distance",
        meta = (DisplayName = "Setpoint",
            ClampMin = 10, UIMin = 10, ClampMax = 50000, UIMax = 50000))
    float CameraDistance_Setpoint = 2000.f;

    /* Скорость изменения Задания дальности камеры */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Camera|Distance",
        meta = (DisplayName = "Setpoint Change Speed",
            ClampMin = 0.01f, UIMin = 0.01f, ClampMax = 1, UIMax = 1))
    float CameraDistance_SetpointChangeSpeed = 0.1f;

    /* Скорость Поворота камеры */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Camera|Rotation",
        meta = (DisplayName = "Turn Speed",
            ClampMin = -10.f, UIMin = -10.f, ClampMax = 10.f, UIMax = 10.f))
    float CameraRotation_TurnSpeed = 1.f;

    /* Диапазон Обзора Камерой по вертикали (вверх-вниз) в градусах */
    UPROPERTY(EditAnywhere,
        Category = "RTS Character|Inputs|Camera|Rotation",
        meta = (DisplayName = "Look Up Range",
            ClampMin = -89, UIMin = -89, ClampMax = 89, UIMax = 89))
    FVector2D CameraRotation_LookUpRange = FVector2D(-89.f, 10.f);
    //-------------------------------------------



private:

    /* ---   Inputs | Movement   --- */

    /** Флаг управления через наведение мыши на Край Экрана */
    bool bScreenEdgeControl = true;

    /* Текущее Окно просмотра Пользователя
    @note   Используется для уменьшения количества операций при отслеживании мыши,
            а также для отслеживания контроллера соответствующего типа */
    FViewport* CurrentViewport = nullptr;

    /* Текущий размера экрана (может изменяться в процессе игры) */
    FIntPoint CurrentSize = FIntPoint::ZeroValue;

    /* Расчитанная Зона Чувствительности для управления через наведение мыши на Края Экрана */
    FVector2D SensitiveZone = FVector2D::ZeroVector;

    //

    /** Ввод передвижения: вперёд-назад */
    virtual void MoveForward(float Value) override;

    /** Ввод передвижения: вправо-влево */
    virtual void MoveRight(float Value) override;

    /** Ввод передвижения: вверх-вниз */
    virtual void MoveUp_World(float Val) override;

    /** Управление с помощью Краёв Экрана */
    void ScreenEdgeControl(float DeltaTime);

    /** Инициализация: Управление краем экрана */
    void InitScreenEdgeControl();

    /** Событме: При изменении размера окна просмотра */
    void OnViewportResized(FViewport* Viewport, uint32 Params);
    //-------------------------------------------



    /* ---   Inputs | Camera   --- */

    /** Обзора Камерой по горизонтали (вправо-влево) с заданной скоростью */
    virtual void TurnAtRate(float Rate) override;

    /** Обзора Камерой по вертикали (вверх-вниз) с заданной скоростью */
    virtual void LookUpAtRate(float Rate) override;

    /** Ввод перемещения Камеры */
    void CameraRange(float Value);

    /** Контроль Высоты Камеры */
    void CameraRangeControl(float DeltaTime);
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITOR

public:

    /* ---   Debugs   --- */

    /** Вызывается, когда свойство этого объекта было изменено извне */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    //-------------------------------------------

#endif // WITH_EDITOR
    //===========================================
};
