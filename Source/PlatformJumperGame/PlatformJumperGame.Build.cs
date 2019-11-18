// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

using UnrealBuildTool;

public class PlatformJumperGame : ModuleRules
{
	public PlatformJumperGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem" });
	}
}
