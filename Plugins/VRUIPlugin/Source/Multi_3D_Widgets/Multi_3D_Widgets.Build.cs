// All Rights Reserved - 2024 - ReEngage Developments Ltd

using UnrealBuildTool;

public class Multi_3D_Widgets : ModuleRules
{
	public Multi_3D_Widgets(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "ApplicationCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
