// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Hide_Seek : ModuleRules
{
	public Hide_Seek(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG" });
	}
}
