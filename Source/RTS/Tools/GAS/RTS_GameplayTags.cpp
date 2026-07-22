// 

// Base:
#include "RTS_GameplayTags.h"

// For visualization via 'Intelli Sense'
#if WITH_EDITOR
#include "RTS_AbilitySystemBlueprintLibrary.h"
#endif // WITH_EDITOR
//--------------------------------------------------------------------------------------



/* ---   Tags   --- */

namespace RTS_GameplayTags
{
    /* ---   Ability Data | Basic   --- */

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AbilityData_Basic_Level, "AbilityData.Basic.Level",
        "Basic parameter: Executed Effect Level");
    //-------------------------------------------


    /* ---   Ability Data | Explosion   --- */

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AbilityData_Explosion_Power, "AbilityData.Explosion.Power",
        "Explosion parameter: Maximum exposure Force (max Power)");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AbilityData_Explosion_DamageRadius, "AbilityData.Explosion.DamageRadius",
        "Explosion parameter: Maximum impact Radius of explosion");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(AbilityData_Explosion_EffectiveRadius, "AbilityData.Explosion.EffectiveRadius",
        "Explosion parameter: Effective explosion radius with Max Power");
    //-------------------------------------------


    /* ---   Gameplay State | Health   --- */

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayState_Health_Max, "GameplayState.Health.Max",
        "Gameplay State: Max Health");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayState_Health_Zero, "GameplayState.Health.Zero",
        "Gameplay State: Zero Health (On Destroyed)");
    //-------------------------------------------


    /* ---   Gameplay State | Armor   --- */

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayState_Armor_Max, "GameplayState.Armor.Max",
        "Gameplay State: Max Armor");

    UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayState_Armor_Zero, "GameplayState.Armor.Zero",
        "Gameplay State: Zero Armor (Without Armor)");
    //-------------------------------------------
}
//--------------------------------------------------------------------------------------
