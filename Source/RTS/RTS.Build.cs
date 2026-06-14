//

using UnrealBuildTool;

public class RTS : ModuleRules
{
    public RTS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {

            // Base:
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",

            // Core:
            "PhysicsCore",

            // Niagara:
            "Niagara",
            "NiagaraCore",

            // Gameplay Ability System (GAS):
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",

            // AI Sense:
            "AIModule"
        });

        if (Target.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {

            // Scripting:
            "EditorScriptingUtilitiesExpansion",
            });
        }
    }
}
