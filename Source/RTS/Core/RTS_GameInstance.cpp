//

// Base:
#include "RTS_GameInstance.h"

// UE:
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
//--------------------------------------------------------------------------------------



/* ---   Statics   --- */

// Общедоступный указатель на текущий экземпляр 'URTS_GameInstance'
URTS_GameInstance* URTS_GameInstance::CurrentGameInstance = nullptr;
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void URTS_GameInstance::Init()
{
    InitSettingsSaving();

    Super::Init();
}
//--------------------------------------------------------------------------------------



/* ---   Settings System | Saving   --- */

bool URTS_GameInstance::SaveSettingsData(const FSettingsData& Data)
{
    if (Data != SaveSettings->SettingsData)
    {
        SaveSettings->SettingsData = Data;
        UGameplayStatics::SaveGameToSlot(SaveSettings, "SettingsData", 0);
    }

    return false;
}

void URTS_GameInstance::InitSettingsSaving()
{
    SaveSettings = Cast<USaveSettings>(UGameplayStatics::LoadGameFromSlot("SettingsData", 0));

    if (!SaveSettings)
    {
        UGameUserSettings* lGameUserSettings = GEngine->GetGameUserSettings();

        /* ---   Auto settings:   --- */
        // Автоматическое определение, принятие и сохранение всех Настроек Видео:
        lGameUserSettings->RunHardwareBenchmark();          // Определение
        lGameUserSettings->ApplyHardwareBenchmarkResults(); // Принятие и Сохранение
        //-------------------------------------------


        /* ---   Default Video settings:   --- */

        // Полный экран
        lGameUserSettings->SetFullscreenMode(EWindowMode::Fullscreen);

        // Разрешение экрана
        TArray<FIntPoint> lAllRes; // Все доступные вариации Разрешения экрана (от меньшего к большему)
        if (lAllRes.Num()) // Нет гарантии заполненности Массива
        {
            // Принятие последнего (наибольшего) значения Разрешения экрана
            lGameUserSettings->SetScreenResolution(lAllRes[lAllRes.Num() - 1]);
        }

        // "Frame Per Second"
        lGameUserSettings->SetFrameRateLimit(60);


        lGameUserSettings->ApplySettings(false);
        //-------------------------------------------


        /* ---   Settings System:   --- */

        // Создание и сохранение базовых Настроек: Игровой процесс, Звук
        SaveSettings = Cast<USaveSettings>(UGameplayStatics::CreateSaveGameObject(
            USaveSettings::StaticClass()));

        UGameplayStatics::SaveGameToSlot(SaveSettings, "SettingsData", 0);
        //-------------------------------------------


        /* ---   Default Sounds settings:   --- */

        if (SoundMix)
        {
            UGameplayStatics::PushSoundMixModifier(GetWorld(), SoundMix);

            if (MusicSoundClass)
            {
                UGameplayStatics::SetSoundMixClassOverride(
                    GetWorld(),
                    SoundMix,
                    MusicSoundClass,
                    GetSettingsData().OverallSoundsVolume * GetSettingsData().MusicSoundsVolume,
                    1.f,
                    0.f);
            }
            else
            {
                M_Error("MusicSoundClass is NOT");
            }

            if (EffectsSoundClass)
            {
                UGameplayStatics::SetSoundMixClassOverride(
                    GetWorld(),
                    SoundMix,
                    MusicSoundClass,
                    GetSettingsData().OverallSoundsVolume * GetSettingsData().EffectSoundsVolume,
                    1.f,
                    0.f);
            }
            else
            {
                M_Error("EffectsSoundClass is NOT");
            }
        }
        else
        {
            M_Error("SoundMix is NOT");
        }
        //-------------------------------------------
    }
}
//--------------------------------------------------------------------------------------
