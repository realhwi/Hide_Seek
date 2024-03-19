// All Rights Reserved - 2023 - ReEngage Developments Ltd

using System.IO;
using UnrealBuildTool;

public class VR_UI : ModuleRules
{
	public VR_UI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "XRBase", "Multi_3D_Widgets" });
    }
}
