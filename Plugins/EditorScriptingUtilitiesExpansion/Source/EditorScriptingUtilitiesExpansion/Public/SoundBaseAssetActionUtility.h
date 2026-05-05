//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "Blutility/Classes/AssetActionUtility.h"

// Generated:
#include "SoundBaseAssetActionUtility.generated.h"
//--------------------------------------------------------------------------------------



UCLASS(Abstract, HideCategories = Object, Blueprintable)
class EDITORSCRIPTINGUTILITIESEXPANSION_API USoundBaseAssetActionUtility : public UAssetActionUtility
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Возвращает класс, который поддерживает данное действие с активом
    (если оно не реализовано, оно будет отображаться для всех типов активов) */
    UClass* GetSupportedClass_Implementable() const { return USoundBase::StaticClass(); };
    //-------------------------------------------



    /* ---   For Single Element   --- */

    /** Установить Класс Звука для указанного Ассета типа "USoundBase"
    @param  SoundBase -- Ассет типа "USoundBase", для которого требуется сменить Класс Звука
    @param  NewClass -- Новый Класс Звука для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Sound Base",
        meta = (DefaultToSelf, HideSelfPin = true))
    static bool SetSoundClass(USoundBase* SoundBase, USoundClass* NewClass);

    /** Установить Настройки Затухания для указанного Ассета типа "USoundBase"
    @param  SoundBase -- Ассет типа "USoundBase", для которого требуется сменить Настройки Затухания
    @param  NewAttenuation -- Новые Настройки Затухания для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Sound Base",
        meta = (DefaultToSelf, HideSelfPin = true))
    static bool SetAttenuationSettings(USoundBase* SoundBase, USoundAttenuation* NewAttenuation);
    //-------------------------------------------



    /* ---   For Multiple Elements   --- */

    /** Установить Класс Звука для выбранных Ассетов типа "USoundBase"
    @param  NewSoundClass -- Новый Класс Звука для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Sound Base",
        meta = (DisplayName = "Set Sound Class",
            DefaultToSelf, HideSelfPin = true))
    static void SetSoundClassOfSelectedAssets(USoundClass* NewSoundClass);

    /** Установить Настройки Затухания для выбранных Ассетов типа "USoundBase"
    @param  NewAttenuation -- Новые Настройки Затухания для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Sound Base",
        meta = (DisplayName = "Set Attenuation Settings",
            DefaultToSelf, HideSelfPin = true))
    static void SetAttenuationSettingsOfSelectedAssets(USoundAttenuation* NewAttenuation);

    /** Установить несколько параметров для выбранных Ассетов типа "USoundBase"
    @param  NewSoundClass -- Новый Класс Звука для данного Ассета
    @param  NewAttenuation -- Новые Настройки Затухания для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Sound Base",
        meta = (DisplayName = "Multiple Set Parameters",
            DefaultToSelf, HideSelfPin = true))
    static void MultipleSetParametersOfSelectedAssets(USoundClass* NewSoundClass, USoundAttenuation* NewAttenuation);
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    /** Проверка класса Ассета */
    FORCEINLINE static bool CheckAssetClass(const UObject* Asset)
    {
        if (Asset)
        {
            if (Cast<USoundBase>(Asset))
            {
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("'%s': '%s' is NOT 'USoundBase'"),
                    *FString(__FUNCTION__), *Asset->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("'%s': 'Asset' is 'nullptr'"),
                *FString(__FUNCTION__));
        }

        return false;
    }
    //-------------------------------------------
};
