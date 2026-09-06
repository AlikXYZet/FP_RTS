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

    /* ---   Attack   --- */

    /* Радиус Патрулирования */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character Data: Attack",
        meta = (ClampMin = "0", UIMin = "0"))
    float PatrolRadius = 500.f;

    /* Дистанция Атаки */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character Data: Attack",
        meta = (ClampMin = "0", UIMin = "0"))
    float AttackDistance = 100.f;

    /* Anim Montage: Атака */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character Data: Attack")
    UAnimMontage* AM_Attack = nullptr;
    //-------------------------------------------



    /* ---   Animation   --- */

    /** BS: Передвижение с зависимостью от скорости */
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Unit Character Data: Animation")
    UBlendSpaceBase* BS_MovementOnSpeed = nullptr;
    //-------------------------------------------
};
//--------------------------------------------------------------------------------------
