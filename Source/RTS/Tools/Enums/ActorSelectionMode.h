//

#pragma once

// Core:
#include "CoreMinimal.h"

// Global:
#include "RTS/Tools/Global/GlobalMacros.h"

// Generated:
#include "ActorSelectionMode.generated.h"
//--------------------------------------------------------------------------------------



/* ---   Enum   --- */

UENUM(BlueprintType)
enum struct EActorSelectionMode : uint8
{
    NotSelected = 0     UMETA(DisplayName = "Not Selected"),

    ControlledFriend,
    ControlledEnemy,
    ControlledNeutral,

    TargetFriend = ControlledFriend + MAX_int8,
    TargetEnemy,
    TargetNeutral,
};

ENUM_COMPARISON_OPERATORS(EActorSelectionMode);
//-------------------------------------------
