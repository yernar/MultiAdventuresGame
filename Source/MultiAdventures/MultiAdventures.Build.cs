// Copyright (C) 2019 Yernar Aldabergenov. All Rights Reserved.

using UnrealBuildTool;

public class MultiAdventures : ModuleRules
{
	public MultiAdventures(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "OnlineSubsystem", "OnlineSubsystemSteam", "SlateCore", "PhysXVehicles", "Steamworks" });
	}
}
