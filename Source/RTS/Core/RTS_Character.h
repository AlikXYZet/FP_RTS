//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/Pawn.h"

// Generated:
#include "RTS_Character.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// UE:
class UCameraComponent;
class UFloatingPawnMovement;
class USpringArmComponent;
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_Character : public APawn
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



    /* ---   Non-scene Components   --- */

    /* Компонент плавного Перемещения */
    UPROPERTY(EditAnywhere, BlueprintReadOnly,
        Category = "Components",
        meta = (AllowPrivateAccess = "true"))
    UFloatingPawnMovement* FloatingMovement = nullptr;
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Вызывается при Запуске игры или при Спавне в уже запущенной игре
    //virtual void BeginPlay() override;
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

    /** Получить текущий Компонент Перемещения персонажа */
    virtual UPawnMovementComponent* GetMovementComponent() const override
    {
        return (UPawnMovementComponent*)FloatingMovement;
    }

    /** Вызывается для привязки к входным данным */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    //-------------------------------------------



    /* ---   Inputs | Actions   --- */

    // Группа Действий для "Test"
    //UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
    //    Category = "RTS_Character|Inputs|Actions",
    //    meta = (GetOptions = "GetActionGroupsNames",
    //        DisplayName = "Test"))
    //FName ActionGroups_Test = NAME_None;
    //-------------------------------------------



    /* ---   Inputs | Axis   --- */

    // Группа Осей для "Движения Вперёд" (вперёд-назад)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS_Character|Inputs|Axis",
        meta = (GetOptions = "GetAxisGroupsNames",
            DisplayName = "Move Forward"))
    FName AxisGroups_MoveForward = NAME_None;

    // Группа Осей для "Движения Вправо" (вправо-влево)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS_Character|Inputs|Axis",
        meta = (GetOptions = "GetAxisGroupsNames",
            DisplayName = "Move Right"))
    FName AxisGroups_MoveRight = NAME_None;

    // Группа Осей для задания Высоты Камеры
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS_Character|Inputs|Axis",
        meta = (GetOptions = "GetAxisGroupsNames",
            DisplayName = "Camera Height"))
    FName AxisGroups_CameraHeight = NAME_None;
    //-------------------------------------------



    /* ---   Inputs | Movement   --- */

    /* Процент зоны чувствительности для управления через наведение мыши на Края Экрана */
    UPROPERTY(EditAnywhere,
        Category = "RTS_Character|Inputs|Movement",
        meta = (ClampMin = 0, UIMin = 0, ClampMax = 0.5f, UIMax = 0.5f))
    FVector2D SensitiveZonePercentage = FVector2D(0.2f);

    /* Скорость перемещения */
    UPROPERTY(EditAnywhere,
        Category = "RTS_Character|Inputs|Movement",
        meta = (ClampMin = 1.f, UIMin = 1.f, ClampMax = 100.f, UIMax = 100.f))
    float MovementSpeed = 20.f;

    //

    /** Установить контроль управления через наведение мыши на Край Экрана */
    UFUNCTION(BlueprintCallable,
        Category = "RTS_Character|Inputs|Movement")
    void SetScreenEdgeControl(bool Value)
    {
        bScreenEdgeControl = Value;
    };
    //-------------------------------------------



    /* ---   Inputs | Camera   --- */

    /* Диапазон Высоты Камеры */
    UPROPERTY(EditAnywhere,
        Category = "RTS_Character|Inputs|Camera",
        meta = (DisplayName = "Range",
            ClampMin = 10, UIMin = 10, ClampMax = 50000, UIMax = 50000))
    FVector2D CameraHeight_Range = FVector2D(10, 5000);

    // Скорость изменения Высоты Камеры
    UPROPERTY(EditAnywhere,
        Category = "RTS_Character|Inputs|Camera",
        meta = (DisplayName = "Change Speed",
            ClampMin = 0.1f, UIMin = 0.1f, ClampMax = 100, UIMax = 100))
    float CameraHeight_ChangeSpeed = 10;

    // Заданная высота камеры
    UPROPERTY(EditAnywhere,
        Category = "RTS_Character|Inputs|Camera",
        meta = (DisplayName = "Setpoint",
            ClampMin = 10, UIMin = 10, ClampMax = 50000, UIMax = 50000))
    float CameraHeight_Setpoint = 2000;

    // Скорость изменения Задания высоты камеры
    UPROPERTY(EditAnywhere,
        Category = "RTS_Character|Inputs|Camera",
        meta = (DisplayName = "Setpoint Change Speed",
            ClampMin = 0.01f, UIMin = 0.01f, ClampMax = 1, UIMax = 1))
    float CameraHeight_SetpointChangeSpeed = 0.1f;
    //-------------------------------------------



private:

    /* ---   Inputs | Movement   --- */

    /** Флаг управления через наведение мыши на Край Экрана */
    bool bScreenEdgeControl = true;

    /* Текущее Окно просмотра Пользователя
    @note   Используется для уменьшения количества операций при отслеживании мыши,
            а также для отслеживания контроллера соответствующего типа */
    FViewport* CurrentViewport = nullptr;

    //

    /** Ввод передвижения: вперёд-назад */
    void MoveForward(float Value);

    /** Ввод передвижения: вправо-влево */
    void MoveRight(float Value);

    /** Управление с помощью Краёв Экрана */
    void ScreenEdgeControl();
    //-------------------------------------------



    /* ---   Inputs | Camera   --- */

    /** Ввод перемещения Камеры */
    void MoveCamera(float Value);

    /** Контроль Высоты Камеры */
    void CameraHeightControl(float DeltaTime);
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITOR

private:

    /* ---   Inputs   --- */

    /* Получить имена зарегистрированных Групп Действий */
    UFUNCTION()
    TArray<FName> GetActionGroupsNames();

    /* Получить имена зарегистрированных Групп Осей */
    UFUNCTION()
    TArray<FName> GetAxisGroupsNames();

    /* Проверить группы входных данных */
    void CheckInputsGroups();
    //-------------------------------------------

#endif // WITH_EDITOR
    //===========================================
};
