// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Hide_Seek : ModuleRules
{
	public Hide_Seek(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HeadMountedDisplay","UMG" , "Niagara" });

		PrivateDependencyModuleNames.AddRange(new string[] { "XRBase", "CableComponent" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
