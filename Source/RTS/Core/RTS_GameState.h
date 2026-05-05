//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/GameStateBase.h"

// Global:
#include "RTS/Tools/GlobalMacros.h"

// Generated:
#include "RTS_GameState.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Static Functions:
static ARTS_GameStateBase* const GetRTSGameState();
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_GameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:

    /* ---   Statics   --- */

    // Общедоступный указатель на текущий экземпляр класса 'ARTS_GameState'
    // @note    Используется для уменьшения зависимостей и использования излишних функций
    //          Например, функций 'Cast<>' и методов Инициализации в других классах
    static ARTS_GameStateBase* CurrentGameState;

    //

    /** Метод проверки валидности статического указателя 'Current Game State' */
    FORCEINLINE static bool IsValidStaticPointer()
    {
        if (!IsValid(CurrentGameState))
        {
            RTS_LOG_Empty(Error,
                "Current GameState is NOT 'ARTS_GameStateBase' class. "
                "See Settings of current 'Game Mode'");

            return false;
        }
        return true;
    };

    /** Метод проверки валидности статического указателя 'Current Game State' */
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

    ARTS_GameStateBase();

    ~ARTS_GameStateBase()
    {
        CurrentGameState = nullptr;
    };
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    // Вызывается при Запуске игры или при Спавне в уже запущенной игре
    virtual void BeginPlay() override;
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Функция, вызываемая каждый кадр в этом Акторе, если не назначена другая частота */
    //virtual void Tick(float DeltaSeconds) override;

    /** Вызывается при размещении экземпляра данного класса (в редакторе) или его запуске.
    @param	Transform - Трансформация данного Актора */
    virtual void OnConstruction(const FTransform& Transform) override;

    /** Вызывается после инициализации всех компонентов только во время игрового процесса */
    //virtual void PostInitializeComponents() override;

    /** Вызывается, когда этот субъект явно уничтожается во время игрового процесса или в редакторе,
    * но не вызывается во время трансляции уровней или завершения игрового процесса */
    virtual void Destroyed() override;
    //-------------------------------------------



private:

    /* ---   Statics   --- */

    /** Получить текущий экземпляр класса 'ARTS_GameState' в среде 'Blueprint' */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game",
        meta = (DisplayName = "Get RTS Game State"))
    static class ARTS_GameStateBase* BP_GetRTSGameState()
    {
        return GetRTSGameState();
    };
    //-------------------------------------------



    /* ---   friends   --- */

    friend class ARTS_GameModeBase;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

/** Получить текущий экземпляр класса 'ARTS_GameState' */
FORCEINLINE static ARTS_GameStateBase* const GetRTSGameState()
{
#if WITH_EDITOR

    if (!ARTS_GameStateBase::CurrentGameState)
    {
        return ARTS_GameStateBase::CurrentGameState = GEngine->GameViewport->GetWorld()->GetGameState<ARTS_GameStateBase>();
    }
    else

#endif // WITH_EDITOR

    {
        // В режиме "Play In Editor" данный указатель очищается, однако стабильно работает в готовой сборке
        return ARTS_GameStateBase::CurrentGameState;
    }
};
//--------------------------------------------------------------------------------------
