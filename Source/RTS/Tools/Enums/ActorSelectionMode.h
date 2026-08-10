//

#pragma once

// Core:
#include "CoreMinimal.h"

// Global:
#include "GlobalMacros.h"

// Generated:
#include "ActorSelectionMode.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Enum   --- */

UENUM(BlueprintType)
enum struct EActorSelectionMode : uint8
{
    NotSelected = 0,

    ControlledFriend,
    ControlledEnemy,
    ControlledNeutral,

    TargetFriend = ControlledFriend + MAX_int8,
    TargetEnemy,
    TargetNeutral,
};

ENUM_COMPARISON_OPERATORS(EActorSelectionMode);
//-------------------------------------------
