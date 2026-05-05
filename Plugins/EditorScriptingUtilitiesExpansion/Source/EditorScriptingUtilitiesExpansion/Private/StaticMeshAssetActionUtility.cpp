//

// Base:
#include "StaticMeshAssetActionUtility.h"

// UE:
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"
//--------------------------------------------------------------------------------------



/* ---   For Single Element   --- */

bool UStaticMeshAssetActionUtility::SetSimpleCollisionPhysicalMaterial(UStaticMesh* StaticMesh, UPhysicalMaterial* PhysMaterial)
{
    if (StaticMesh
        && StaticMesh->GetBodySetup())
    {
        if (StaticMesh->GetBodySetup()->PhysMaterial != PhysMaterial)
        {
            StaticMesh->GetBodySetup()->PhysMaterial = PhysMaterial;
            StaticMesh->MarkPackageDirty();
        }

        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------



/* ---   For Multiple Elements   --- */

void UStaticMeshAssetActionUtility::SetSimpleCollisionPhysicalMaterialOfSelectedAssets(UPhysicalMaterial* PhysMaterial)
{
    for (UObject* Asset : UEditorUtilityLibrary::GetSelectedAssets())
    {
        if (CheckAssetClass(Asset))
        {
            SetSimpleCollisionPhysicalMaterial((UStaticMesh*)Asset, PhysMaterial);
        }
    }
}
//--------------------------------------------------------------------------------------
