//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/PlayerController.h"

// Generated:
#include "RTS_PlayerController.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат: При Нажатии на Компонент (при нажатии клавиш из списка 'Click Event Keys' на какой-нибудь компонент)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClickingOnComponent, const UPrimitiveComponent*, ClickedPrimitive, FKey, Key);
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
//class UName;
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    // Делегат: При Нажатии на Компонент (при нажатии клавиш из списка 'Click Event Keys' на какой-нибудь компонент)
    UPROPERTY(BlueprintAssignable)
    FOnClickingOnComponent OnClickingOnComponent;
    //-------------------------------------------



    /* ---   Constructors   --- */

    // Sets default values for this actor's properties
    ARTS_PlayerController();
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
    //-------------------------------------------



    /* ---   Mouse   --- */

    /** Включить события мыши */
    FORCEINLINE void EnableMouseEvents(bool bControl)
    {
        // Выключение реакций от наведения мыши данным контроллером
        bEnableMouseOverEvents = bControl;

        // Выключение реакций от нажатия мыши данным контроллером
        bEnableClickEvents = bControl;
    };

    /** Установить контроль мыши в центр экрана */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Player Controller|Mouse")
    void SetMouseControlToCenter(bool Value)
    {
        bMouseToCenter = Value;
    };
    //-------------------------------------------



    /* ---   Action   --- */

    // Выбранные группы Действий
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Action",
        meta = (GetOptions = "GetActionNames"))
    TArray<FName> SelectedActionGroups;

    //

    /** Получить группы клавиш воздействия контроллера */
    TArray<FName> GetActionsGroup(FKey Key);
    //-------------------------------------------



private:

    /* ---   Mouse To Center   --- */

    // Текущая позиция мыши по оси X
    float MousePositionX = 0;

    // Текущая позиция мыши по оси Y
    float MousePositionY = 0;

    // Текущий центр экрана по оси X
    int32 SizeCenterX = 0;

    // Текущий центр экрана по оси Y
    int32 SizeCenterY = 0;

    // Флаг контроля Мыши в центре Экрана
    bool bMouseToCenter = false;

    //

    /** Установить Мышь в центр экрана */
    void SetMouseToCenter();
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITOR

public:

    /* ---   Debugs   --- */

    /** Вызывается, когда свойство этого объекта было изменено извне */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    //-------------------------------------------



private:

    /* ---   Actions   --- */

    /* Предварительная инициализация Клавиш, используемых с прицеливанием (наведение мышью и нажатие клавиши) */
    void InitActionGroup();

    /* Получить имена всех Функций-Предикатов Актора-Владельца */
    UFUNCTION()
    TArray<FName> GetActionNames();
    //-------------------------------------------
#endif
    //===========================================
};
