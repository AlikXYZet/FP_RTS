//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/PlayerController.h"

// Global:
#include "RTS/Tools/GlobalMacros.h"

// Generated:
#include "RTS_PlayerController.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Static Functions:
static ARTS_PlayerController* const GetRTSLocalController();

// Interaction:
class AUnitCharacter;
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_PlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    /* ---   Statics   --- */

    // Общедоступный указатель на текущий Локальный Контроллер класса 'ARTS_PlayerController'
    // @note    Используется для уменьшения зависимостей и использования излишних функций
    //          Например, функций 'Cast<>' и методов Инициализации в других классах
    static ARTS_PlayerController* CurrentLocalController;

    //

    /** Метод проверки валидности статического указателя на Локальный Контроллер класса 'ARTS_PlayerController' */
    FORCEINLINE static bool IsValidStaticPointer()
    {
        if (!IsValid(CurrentLocalController))
        {
            RTS_LOG_Empty(Error,
                "Current GameState is NOT 'ARTS_GameStateBase' class. "
                "See Settings of current 'Game Mode'");

            return false;
        }
        return true;
    };

    /** Метод проверки валидности статического указателя на Локальный Контроллер класса 'ARTS_PlayerController' */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Game",
        meta = (DisplayName = "Is Valid Static Pointer", ExpandBoolAsExecs = "ReturnValue",
            DefaultToSelf))
    bool BP_IsValidStaticPointer()
    {
        return IsValidStaticPointer();
    };
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



    /* ---   Inputs | Actions   --- */

    /** Вызывается для привязки к входным данным */
    virtual void SetupInputComponent() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Функция, вызываемая каждый кадр в этом Акторе, если не назначена другая частота */
    virtual void Tick(float DeltaSeconds) override;

    /** Вызывается, когда этот субъект явно уничтожается во время игрового процесса или в редакторе,
    * но не вызывается во время трансляции уровней или завершения игрового процесса */
    virtual void Destroyed() override;
    //-------------------------------------------



    /* ---   Inputs | Actions   --- */

    /* Группа Действий для "Экранный Выбор" */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Player Controller|Inputs|Actions",
        meta = (GetOptions = "GetActionGroupsNames",
            DisplayName = "On-Screen Selection"))
    FName ActionGroups_OnScreenSelection = NAME_None;

    /* Группа Действий для "Экранное Действие" */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
        Category = "RTS Player Controller|Inputs|Actions",
        meta = (GetOptions = "GetActionGroupsNames",
            DisplayName = "On-Screen Action"))
    FName ActionGroups_OnScreenAction = NAME_None;

    /* Выбранные группы Действий для отслеживания Клавиш
    @note   Заполняет парамметр 'Click Event Keys' клавишами из данных Групп */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Inputs|Actions",
        meta = (GetOptions = "GetActionGroupsNames",
            DisplayName = "Other Screen Interactions"))
    TArray<FName> ActionGroups_OtherScreenInteractions;

    //

    /** Вызывается при нажатии Клавиш Событий из списка параметра 'Click Event Keys' */
    virtual bool InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad) override;
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

    /** Получить текущий размер Окна */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Player Controller|Mouse")
    FIntPoint GetCurrentViewportSize()
    {
        if (CurrentViewport)
        {
            return CurrentViewport->GetSizeXY();
        }
        return FIntPoint::NoneValue;
    };

    /** Получить текущую Позицию мыши в пределах Окна */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Player Controller|Mouse")
    FIntPoint GetCurrentMousePosition()
    {
        FIntPoint lPos = FIntPoint::NoneValue;
        if (CurrentViewport)
        {
            CurrentViewport->GetMousePos(lPos);
        }
        return lPos;
    };
    //-------------------------------------------



    /* ---   Action   --- */

    /* Результат Попадания для Групп Действий отслеживаемых Клавиш */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Action")
    FHitResult HitResultForActionGroups;
    //-------------------------------------------



    /* ---   Selectable Actor   --- */

    /* Массив Выбранных Союзных Юнитов */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Selectable Actor")
    TSet<AUnitCharacter*> SelectedAlliedUnits;

    /* Выбранный Актор целевого Действия */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Selectable Actor")
    AActor* SelectedTargetActionActor;

    /* Номер Фракции данного Юнита */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Selectable Actor")
    uint8 FractionNumber = 0;

    //

    /** Очистить список отлеживаемых Юнитов */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Player Controller|Selectable Actor")
    void ClearSelectedUnits();
    //-------------------------------------------



private:

    /* ---   Statics   --- */

    /** Получить текущий Локальный Контроллер класса 'ARTS_PlayerController' в среде 'Blueprint' */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game",
        meta = (DisplayName = "Get RTS Local Controller"))
    static class ARTS_PlayerController* BP_GetRTSLocalController()
    {
        return GetRTSLocalController();
    };
    //-------------------------------------------



    /* ---   Inputs | Actions   --- */

    /** Действие при "Экранном Выборе" */
    UFUNCTION()
    void OnScreenSelection();

    /** Действие при "Экранном Воздействии" */
    UFUNCTION()
    void OnScreenAction();
    //-------------------------------------------



    /* ---   Mouse   --- */

    // Флаг контроля Мыши в центре Экрана
    bool bMouseControlToCenter = false;

    /* Текущее Окно просмотра Пользователя
    @note   Используется для уменьшения количества операций при контроле мыши */
    FViewport* CurrentViewport = nullptr;

    //

    /** Инициализация данных контроля мыши */
    void InitMouseControl();

    /** Удерживать Мышь в центр экрана */
    void KeepMouseCentered();
    //-------------------------------------------



    /* ---   Selectable Actor   --- */

    /* Выбрать другой Актор для Действия */
    void SetSelectedTargetActionActor(AActor* TargetActor);
    //-------------------------------------------



    /* ===   For EDITOR only   === */

#if WITH_EDITOR

public:

    /* ---   Debugs   --- */

    /** Вызывается, когда свойство этого объекта было изменено извне */
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
    //-------------------------------------------



public:

    /* ---   Inputs | Actions   --- */

    /* Предварительная инициализация Клавиш, используемых для взаимодействия (нажатие клавиши в момент наведения мышью) */
    void UpdateClickEventKeys();

    /* Получить имена всех Функций-Предикатов Актора-Владельца */
    UFUNCTION()
    TArray<FName> GetActionGroupsNames();
    //-------------------------------------------
#endif
    //===========================================
};
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

/** Получить текущий Локальный Контроллер класса 'ARTS_PlayerController' */
FORCEINLINE static ARTS_PlayerController* const GetRTSLocalController()
{
#if WITH_EDITOR

    if (!ARTS_PlayerController::CurrentLocalController)
    {
        for (FConstPlayerControllerIterator Iterator = GEngine->GameViewport->GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            if (APlayerController* PlayerController = Iterator->Get())
            {
                // Проверка локальности контроллера
                if (Cast<ULocalPlayer>(PlayerController->Player))
                {
                    return ARTS_PlayerController::CurrentLocalController = Cast<ARTS_PlayerController>(PlayerController);
                }
            }
        }
        return ARTS_PlayerController::CurrentLocalController;
    }
    else

#endif // WITH_EDITOR

    {
        // В режиме "Play In Editor" данный указатель очищается, однако стабильно работает в готовой сборке
        return ARTS_PlayerController::CurrentLocalController;
    }
};
//--------------------------------------------------------------------------------------
