// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Proj_proto : ModuleRules
{
	public Proj_proto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
