//

#pragma once

// Core:
#include "CoreMinimal.h"

// UE:
#include "Animation/BlendSpaceBase.h"

// Generated:
#include "UnitCharacterData.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Pre-declaration of classes   --- */

// Interaction:
//class ;
//--------------------------------------------------------------------------------------



/** Данные Единичного Персонажа отряда */
USTRUCT(BlueprintType)
struct FUnitCharacterData
{
    GENERATED_BODY()

    /* ---   Data   --- */

    /* Дистанция Атаки */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character Data: Attack",
        meta = (ClampMin = "0", UIMin = "0"))
    float AttackDistance = 0.2f;

    /* Anim Montage: Атака */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character Data: Attack")
    UAnimMontage* AM_Attack = nullptr;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
