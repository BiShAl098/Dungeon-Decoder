// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonDecoder : ModuleRules
{
	public DungeonDecoder(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"DungeonDecoder",
			"DungeonDecoder/Variant_Platforming",
			"DungeonDecoder/Variant_Platforming/Animation",
			"DungeonDecoder/Variant_Combat",
			"DungeonDecoder/Variant_Combat/AI",
			"DungeonDecoder/Variant_Combat/Animation",
			"DungeonDecoder/Variant_Combat/Gameplay",
			"DungeonDecoder/Variant_Combat/Interfaces",
			"DungeonDecoder/Variant_Combat/UI",
			"DungeonDecoder/Variant_SideScrolling",
			"DungeonDecoder/Variant_SideScrolling/AI",
			"DungeonDecoder/Variant_SideScrolling/Gameplay",
			"DungeonDecoder/Variant_SideScrolling/Interfaces",
			"DungeonDecoder/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
