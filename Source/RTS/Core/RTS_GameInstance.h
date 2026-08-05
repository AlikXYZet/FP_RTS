//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "Engine/GameInstance.h"

// Global:
#include "GlobalMacros.h"

// Generated:
#include "RTS_GameInstance.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Static Functions:
static URTS_GameInstance* const GetRTSGameInstance();
//--------------------------------------------------------------------------------------



UCLASS()
class RTS_API URTS_GameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    /* ---   Statics   --- */

    // Общедоступный указатель на текущий экземпляр класса 'URTS_GameInstance'
    // @note    Используется для уменьшения зависимостей и использования излишних функций
    //          Например, функций 'Cast<>' и методов Инициализации в других классах
    static URTS_GameInstance* CurrentGameInstance;

    //

    /** Метод проверки валидности статического указателя 'Current Game Instance' */
    FORCEINLINE static bool IsValidStaticPointer()
    {
        if (!IsValid(CurrentGameInstance))
        {
            M_LOG_Empty(Error,
                "Current GameInstance is NOT 'URTS_GameInstance' class. "
                "See 'Project Settings'/'Maps & Modes'/'Game Instance Class'");

            return false;
        }
        return true;
    };

    /** Метод проверки валидности статического указателя 'Current Game Instance' */
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

    // Sets default values for this component's properties
    URTS_GameInstance()
    {
        CurrentGameInstance = this;
    };
    //-------------------------------------------



public:

    /* ---   Base   --- */

    /** Инициализация данных GameInstance */
    virtual void Init() override;
    //-------------------------------------------



    /* ---   Settings System | Sounds   --- */

    // Микшер громкости
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings System: Sounds")
    USoundMix* SoundMix = nullptr;

    // Микшер громкости для Музыки
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings System: Sounds")
    USoundClass* MusicSoundClass = nullptr;

    // Микшер громкости для Эффектов
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Settings System: Sounds")
    USoundClass* EffectsSoundClass = nullptr;
    //-------------------------------------------



private:

    /* ---   Statics   --- */

    /** Получить текущий экземпляр класса 'URTS_GameInstance' в среде 'Blueprint' */
    UFUNCTION(BlueprintPure,
        Category = "RTS Game",
        meta = (DisplayName = "Get RTS Game Instance"))
    static class URTS_GameInstance* BP_GetRTSGameInstance()
    {
        return GetRTSGameInstance();
    };
    //-------------------------------------------



    /* ---   Settings System | Saving   --- */

    /** Инициализация сохранения данных Настроек */
    void InitSettingsSaving();
    //-------------------------------------------
};



/* ---   Statics   --- */

/** Получить текущий экземпляр класса 'URTS_GameInstance' */
FORCEINLINE static URTS_GameInstance* const GetRTSGameInstance()
{
#if WITH_EDITOR

    if (!URTS_GameInstance::CurrentGameInstance)
    {
        // Обход Очистки Указателя в режиме редактора
        // @note    Предположительно, "обнуление" происходит из-за 'Hot Reload'
        if (GEngine->GameViewport && GEngine->GameViewport->GetWorld())
            URTS_GameInstance::CurrentGameInstance = GEngine->GameViewport->GetWorld()->GetGameInstance<URTS_GameInstance>();
    }

#endif // WITH_EDITOR

    // В режиме "Play In Editor" данный 'static'-указатель очищается, даже если будет реализован через умные указатели.
    // Однако стабильно работает в готовой сборке
    return URTS_GameInstance::CurrentGameInstance;
};
//--------------------------------------------------------------------------------------
