//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "GameFramework/GameModeBase.h"

// Macros:
#include "RTS/Tools/GlobalMacros.h"

// Generated:
#include "RTS_GameMode.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Static Functions:
static ARTS_GameModeBase* const GetRTSGameMode();
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API ARTS_GameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:

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
            RTS_LOG_Empty(Error,
                "Current GameMode is NOT 'ARTS_GameModeBase' class. "
                "See 'World Settings'");

            return false;
        }
        return true;
    };

    /** Метод проверки валидности статического указателя 'Current Game Mode' */
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
        return ARTS_GameModeBase::CurrentGameMode = GEngine->GameViewport->GetWorld()->GetAuthGameMode<ARTS_GameModeBase>();
    }
    else

#endif // WITH_EDITOR

    {
        // В режиме "Play In Editor" данный указатель очищается, однако стабильно работает в готовой сборке
        return ARTS_GameModeBase::CurrentGameMode;
    }
};
//--------------------------------------------------------------------------------------
