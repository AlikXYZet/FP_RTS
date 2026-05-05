//

#pragma once

// Core:
#include "CoreMinimal.h"

// Base:
#include "Blutility/Classes/AssetActionUtility.h"

// Generated:
#include "StaticMeshAssetActionUtility.generated.h"
//--------------------------------------------------------------------------------------



UCLASS(Abstract, HideCategories = Object, Blueprintable)
class EDITORSCRIPTINGUTILITIESEXPANSION_API UStaticMeshAssetActionUtility : public UAssetActionUtility
{
    GENERATED_BODY()

public:

    /* ---   Base   --- */

    /** Возвращает класс, который поддерживает данное действие с активом
    (если оно не реализовано, оно будет отображаться для всех типов активов) */
    UClass* GetSupportedClass_Implementable() const { return UStaticMesh::StaticClass(); };
    //-------------------------------------------



    /* ---   For Single Element   --- */

    /** Установить Физику Материала для указанного Ассета типа "UStaticMesh"
    @param  StaticMesh -- Ассет типа "UStaticMesh", для которого требуется сменить Физику Материала
    @param  PhysMaterial -- Физика Материала для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Static Mesh",
        meta = (DefaultToSelf, HideSelfPin = true))
    static bool SetSimpleCollisionPhysicalMaterial(UStaticMesh* StaticMesh, UPhysicalMaterial* PhysMaterial);
    //-------------------------------------------



    /* ---   For Multiple Elements   --- */

    /** Установить Физику Материала для выбранных Ассетов типа "UStaticMesh"
    @param  PhysMaterial -- Физика Материала для данного Ассета */
    UFUNCTION(BlueprintCallable, CallInEditor,
        Category = "Asset Action Utility|Static Mesh",
        meta = (DisplayName = "Set Simple Collision Physical Material",
            DefaultToSelf, HideSelfPin = true))
    static void SetSimpleCollisionPhysicalMaterialOfSelectedAssets(UPhysicalMaterial* PhysMaterial);
    //-------------------------------------------



protected:

    /* ---   Base   --- */

    /** Проверка класса Ассета */
    FORCEINLINE static bool CheckAssetClass(const UObject* Asset)
    {
        if (Asset)
        {
            if (Cast<UStaticMesh>(Asset))
            {
                return true;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("'%s': '%s' is NOT 'UStaticMesh'"),
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
