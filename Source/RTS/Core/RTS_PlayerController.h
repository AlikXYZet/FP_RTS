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



public:

    /* ---   Base   --- */

    /** Функция, вызываемая каждый кадр в этом Акторе, если не назначена другая частота */
    virtual void Tick(float DeltaSeconds) override;

    /** Вызывается, когда этот субъект явно уничтожается во время игрового процесса или в редакторе,
    * но не вызывается во время трансляции уровней или завершения игрового процесса */
    virtual void Destroyed() override;

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

    /* Выбранные группы Действий для отслеживания Клавиш
    @note   Заполняет парамметр 'Click Event Keys' клавишами из данных Групп */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Action",
        meta = (GetOptions = "GetActionNames"))
    TArray<FName> SelectedActionGroups;

    /* Результат Попадания для Групп Действий отслеживаемых Клавиш */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Action")
    FHitResult HitResultForActionGroups;

    //

    /** Получить группы клавиш воздействия контроллера */
    TArray<FName> GetActionsGroup(FKey Key);
    //-------------------------------------------



    /* ---   Selectable Actor   --- */

    /* Массив Выбранных Союзных Юнитов */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Selectable Actor")
    TArray<AUnitCharacter*> SelectedAlliedUnits;
    /* PS: 'TSet' не подходит, так как есть необходимость в манипуляциях в среде 'Blueprint' */

    /* Выбранный целевой Актор Взаимодействия */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Selectable Actor")
    AActor* SelectedActor;

    /* Номер Фракции данного Юнита */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "RTS Player Controller|Selectable Actor")
    uint8 FractionNumber = 0;

    //

    /** Добавить Юнита для отслеживания */
    void AddUnitToSelectedUnits(AActor* Unit);

    /** Убрать Юнита из отслеживания */
    void RemoveUnitFromSelectedUnits(AActor* Unit);

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
