//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/GameModeBase.h"

// Global:
#include "GlobalMacros.h"

// Structs:
#include "GenericTeamAgentInterface.h"
#include "RTS/Tools/Structs/Properties/FactionData.h"

// Generated:
#include "RTS_GameMode.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Static Functions:
static ARTS_GameModeBase* const GetRTSGameMode();

// Interaction:
class AUnitCharacter;
class AAttributedActor;
//--------------------------------------------------------------------------------------



/* ---   Delegates   --- */

// Делегат: При изменении Количества членов Фракции
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangingFactionUnitsNumber, uint8, TeamID, const FFactionData&, FactionData);

// Делегат: При уничтожении Фракции
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFactionDestruction, uint8, TeamID, const FFactionData&, FactionData);
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_GameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:

    /* ---   Delegates   --- */

    // Делегат: При изменении Количества членов Фракции
    UPROPERTY(BlueprintAssignable)
    FOnChangingFactionUnitsNumber OnChangingFactionUnitsNumber;

    // Делегат: При уничтожении Фракции
    UPROPERTY(BlueprintAssignable)
    FOnFactionDestruction OnFactionDestruction;
    //-------------------------------------------



    /* ---   Statics   --- */

    // Общедоступный указатель на текущий экземпляр класса 'ARTS_GameMode'
    // @note    Используется для уменьшения зависимостей и использования излишних функций
    //          Например, функций 'Cast<>' и методов Инициализации в других классах
    static ARTS_GameModeBase* CurrentGameMode;

    //

    /** Метод проверки валидности статического указателя 'Current Game Mode' */
    FORCEINLINE static bool IsValidStaticPointer()
    {
        if (!IsValid(CurrentGameMode))
        {
            M_Error_Empty(
                "Current GameMode is NOT 'ARTS_GameModeBase' class. "
                "See 'World Settings'");

            return false;
        }
        return true;
    };

    /** Метод проверки валидности статического указателя 'Current Game Mode' */
    UFUNCTION(BlueprintCallable, BlueprintPure = false,
        Category = "RTS Game",
        meta = (DisplayName = "Is Valid Static Pointer", ExpandBoolAsExecs = "ReturnValue",
            DefaultToSelf))
    bool BP_IsValidStaticPointer() const
    {
        return IsValidStaticPointer();
    };
    //-------------------------------------------



    /* ---   Constructors   --- */

    ARTS_GameModeBase();

    ~ARTS_GameModeBase()
    {
        CurrentGameMode = nullptr;
    }
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

    /** Вызывается при размещении экземпляра данного класса (в редакторе) или его запуске.
    @param	Transform	- Трансформация данного Актора */
    virtual void OnConstruction(const FTransform& Transform) override;

    /** Вызывается, когда этот субъект явно уничтожается во время игрового процесса или в редакторе,
    * но не вызывается во время трансляции уровней или завершения игрового процесса */
    virtual void Destroyed() override;
    //-------------------------------------------



    /* ---   Statistics   --- */

    /* Таблица Данных: Данные Фракций */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,
        Category = "RTS Game Mode|Statistics",
        meta = (NoResetToDefault, RequiredAssetDataTags = "RowStructure=FactionData"))
    UDataTable* FactionsData = nullptr;

    //

    /** Регистрация Юнита фракции */
    void UnitRegistration(const AUnitCharacter* Unit);

    /** Регистрация уничтожения Юнита фракции */
    bool UnitDestructionRegistration(const AUnitCharacter* Unit);

    /** Регистрация Актора фракции */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Game Mode|Statistics")
    void FactionActorRegistration(const AAttributedActor* Actor);

    /** Регистрация уничтожения Актора фракции */
    UFUNCTION(BlueprintCallable,
        Category = "RTS Game Mode|Statistics")
    bool FactionActorDestructionRegistration(const AAttributedActor* Actor);

    /** Получить данные о всех Фракциях */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game Mode|Statistics")
    const TArray<FFactionData>& GetAllFactions() const
    {
        return AllFactions;
    };

    /** Получить данные о Фракци */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game Mode|Statistics")
    const FFactionData& GetFactionData(const AUnitCharacter* Unit) const;

    /** Получить данные о Фракци по его ID */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game Mode|Statistics")
    const FFactionData& GetFactionDataByID(uint8 TeamID) const;

    /** Получить Колличество чужих Юнитов для Фракции (все, кроме выбранной) */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game Mode|Statistics")
    const int64 GetNumberOfOtherUnits(uint8 IgnoredTeam);
    //-------------------------------------------



private:

    /* ---   Statics   --- */

    /** Получить текущий экземпляр класса 'ARTS_GameMode' в среде 'Blueprint' */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game",
        meta = (DisplayName = "Get RTS Game Mode"))
    static class ARTS_GameModeBase* BP_GetRTSGameMode()
    {
        return GetRTSGameMode();
    };
    //-------------------------------------------



    /* ---   Statistics   --- */

    // Все Юниты Фракций
    TArray<FFactionData> AllFactions;

    //

    /** Инициализация системы Статистики */
    void InitStatistics();
    //-------------------------------------------



    /* ---   friends   --- */

    friend class ARTS_GameStateBase;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

/** Получить текущий экземпляр класса 'ARTS_GameModeBase' */
FORCEINLINE static ARTS_GameModeBase* const GetRTSGameMode()
{
#if WITH_EDITOR

    if (!ARTS_GameModeBase::CurrentGameMode)
    {
        // Обход Очистки Указателя в режиме редактора
        // @note    Предположительно, "обнуление" происходит из-за 'Hot Reload'
        if (GEngine->GameViewport && GEngine->GameViewport->GetWorld())
            ARTS_GameModeBase::CurrentGameMode = GEngine->GameViewport->GetWorld()->GetAuthGameMode<ARTS_GameModeBase>();
    }

#endif // WITH_EDITOR

    // В режиме "Play In Editor" данный 'static'-указатель очищается, даже если будет реализован через умные указатели.
    // Однако стабильно работает в готовой сборке
    return ARTS_GameModeBase::CurrentGameMode;
};
//--------------------------------------------------------------------------------------
