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
    virtual void BeginPlay() override;
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
        // Контроль реакций от наведения мыши данным контроллером
        bEnableMouseOverEvents = bControl;

        // Контроль реакций от нажатия мыши данным контроллером
        bEnableClickEvents = bControl;
    };

    /** Установить контроль мыши в центр экрана */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Player Controller|Mouse")
    void SetMouseControlToCenter(bool Value)
    {
        bMouseControlToCenter = Value;
    };

    /** Установить Мышь в центр экрана */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Player Controller|Mouse")
    void SetMouseToCenter();

    /** Получить текущее Окно просмотра Пользователя */
    FViewport* GetCurrentViewport() const { return CurrentViewport; }
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

    /* ---   Mouse   --- */

    /* Текущее Окно просмотра Пользователя
    @note   Используется для уменьшения количества операций при контроле мыши */
    FViewport* CurrentViewport = nullptr;

    // Флаг контроля Мыши в центре Экрана
    bool bMouseControlToCenter = false;

    //

    /** Инициализация данных контроля мыши */
    void InitMouseControl();

    /** Удерживать Мышь в центр экрана */
    void KeepMouseCentered();
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
