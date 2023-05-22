// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ERA : ModuleRules
{
	public ERA(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "MotionWarping", "NetCore", "Niagara" });
		
		PublicIncludePaths.Add("ERA/");
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
		});
	}
}
