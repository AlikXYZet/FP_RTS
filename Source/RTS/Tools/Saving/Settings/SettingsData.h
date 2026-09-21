//

#pragma once

// Core:
#include "CoreMinimal.h"

// UE:
#include "Kismet/KismetInternationalizationLibrary.h"

// Generated:
#include "SettingsData.generated.h"
//--------------------------------------------------------------------------------------



// Структура данных игры
USTRUCT(BlueprintType)
struct FSettingsData
{
    GENERATED_BODY()

    /* ---   Gameplay   --- */

    /* Флаг отображения Крови */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bBloodIsShown = false;

    /* Флаг отображения Подсказок Управления */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bControlTipsIsShown = true;
    //-------------------------------------------


    /* ---   Sounds   --- */

    /** Общая громкость Звуков */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallSoundsVolume = 0.3f;

    /* Громкость Музыки */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MusicSoundsVolume = 1.0f;

    /* Громкость Эффектов */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectSoundsVolume = 1.0f;
    //-------------------------------------------


    /* ---   Operators | =   --- */

    bool operator!=(const FSettingsData& Second) const
    {
        return bBloodIsShown != Second.bBloodIsShown
            || bControlTipsIsShown != Second.bControlTipsIsShown
            || OverallSoundsVolume != Second.OverallSoundsVolume
            || MusicSoundsVolume != Second.MusicSoundsVolume
            || EffectSoundsVolume != Second.EffectSoundsVolume;
    };
    //-------------------------------------------


    /* ---   Statics   --- */

    /* Пустые Данные Настроек */
    static const FSettingsData Empty;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
