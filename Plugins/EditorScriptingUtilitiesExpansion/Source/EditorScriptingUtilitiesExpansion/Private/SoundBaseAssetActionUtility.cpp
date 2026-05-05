//

// Base:
#include "SoundBaseAssetActionUtility.h"

// UE:
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"
//--------------------------------------------------------------------------------------



/* ---   For Single Element   --- */

bool USoundBaseAssetActionUtility::SetSoundClass(USoundBase* SoundBase, USoundClass* NewClass)
{
    if (SoundBase)
    {
        if (SoundBase->SoundClassObject != NewClass)
        {
            SoundBase->SoundClassObject = NewClass;
            SoundBase->MarkPackageDirty();
        }

        return true;
    }
    return false;
}

bool USoundBaseAssetActionUtility::SetAttenuationSettings(USoundBase* SoundBase, USoundAttenuation* NewAttenuation)
{
    if (SoundBase)
    {
        if (SoundBase->AttenuationSettings != NewAttenuation)
        {
            SoundBase->AttenuationSettings = NewAttenuation;
            SoundBase->MarkPackageDirty();
        }

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------



/* ---   For Multiple Elements   --- */

void USoundBaseAssetActionUtility::SetSoundClassOfSelectedAssets(USoundClass* NewSoundClass)
{
    for (UObject* Asset : UEditorUtilityLibrary::GetSelectedAssets())
    {
        if (CheckAssetClass(Asset))
        {
            SetSoundClass((USoundBase*)Asset, NewSoundClass);
        }
    }
}

void USoundBaseAssetActionUtility::SetAttenuationSettingsOfSelectedAssets(USoundAttenuation* NewAttenuation)
{
    for (UObject* Asset : UEditorUtilityLibrary::GetSelectedAssets())
    {
        if (CheckAssetClass(Asset))
        {
            SetAttenuationSettings((USoundBase*)Asset, NewAttenuation);
        }
    }
}

void USoundBaseAssetActionUtility::MultipleSetParametersOfSelectedAssets(USoundClass* NewSoundClass, USoundAttenuation* NewAttenuation)
{
    for (UObject* Asset : UEditorUtilityLibrary::GetSelectedAssets())
    {
        if (CheckAssetClass(Asset))
        {
            SetSoundClass((USoundBase*)Asset, NewSoundClass);
            SetAttenuationSettings((USoundBase*)Asset, NewAttenuation);
        }
    }
}
//--------------------------------------------------------------------------------------
