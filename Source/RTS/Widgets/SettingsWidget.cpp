//

// Base:
#include "SettingsWidget.h"

// Win:
#if PLATFORM_WINDOWS
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"
#include <windows.h> // Conflict header file
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#endif // PLATFORM_WINDOWS

// UE:
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h"
//--------------------------------------------------------------------------------------



/* ---   Base   --- */

void USettingsWidget::NativeOnInitialized()
{
    InitWidgetData();

    URTS_GameInstance::IsValidStaticPointer();

    Super::NativeOnInitialized();
}

void USettingsWidget::NativeConstruct()
{
    Super::NativeConstruct();
}
//--------------------------------------------------------------------------------------



/* ---   Settings System | Saving   --- */

const int32 USettingsWidget::GetDisplayFrequency() const
{
    int32 lRTS = 60;

    // Ограничение по платформе: Только WINDOWS
#if PLATFORM_WINDOWS
    DEVMODE dm;
    dm.dmSize = sizeof(DEVMODE);
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    lRTS = dm.dmDisplayFrequency;
#endif // PLATFORM_WINDOWS

    return lRTS;
}

void USettingsWidget::LoadSettings()
{
    if (NewSettingsData != GetCurrentSettingsData())
    {
        NewSettingsData = GetCurrentSettingsData();

        UpdateSounds();
    }

    if (bIsVideoSettingsDirty
        || GameUserSettings->IsDirty())
    {
        GameUserSettings->LoadSettings();
        bIsVideoSettingsDirty = false;
    }
}

void USettingsWidget::ApplySettings()
{
    GetRTSGameInstance()->SaveSettingsData(NewSettingsData);

    if (bIsVideoSettingsDirty
        || GameUserSettings->IsDirty())
    {
        GameUserSettings->ValidateSettings();
        GameUserSettings->ApplyHardwareBenchmarkResults();
        GameUserSettings->ApplySettings(false);
        bIsVideoSettingsDirty = false;
    }
}

bool USettingsWidget::IsSettingsDirty()
{
    return bIsVideoSettingsDirty
        || NewSettingsData != GetCurrentSettingsData()
        || GEngine->GetGameUserSettings()->IsDirty();
}

void USettingsWidget::InitWidgetData()
{
    NewSettingsData = GetCurrentSettingsData();
    GameUserSettings = GEngine->GetGameUserSettings();
}
//--------------------------------------------------------------------------------------



/* ---   Settings System | Sounds   --- */

void USettingsWidget::UpdateSounds()
{
    if (GetRTSGameInstance()->SoundMix)
    {
        UGameplayStatics::PushSoundMixModifier(GetWorld(), GetRTSGameInstance()->SoundMix);

        if (GetRTSGameInstance()->MusicSoundClass)
        {
            UGameplayStatics::SetSoundMixClassOverride(
                GetWorld(),
                GetRTSGameInstance()->SoundMix,
                GetRTSGameInstance()->MusicSoundClass,
                GetCurrentSettingsData().OverallSoundsVolume * GetCurrentSettingsData().MusicSoundsVolume,
                1.f,
                0.f);
        }

        if (GetRTSGameInstance()->EffectsSoundClass)
        {
            UGameplayStatics::SetSoundMixClassOverride(
                GetWorld(),
                GetRTSGameInstance()->SoundMix,
                GetRTSGameInstance()->MusicSoundClass,
                GetCurrentSettingsData().OverallSoundsVolume * GetCurrentSettingsData().EffectSoundsVolume,
                1.f,
                0.f);
        }
    }
}
//--------------------------------------------------------------------------------------
