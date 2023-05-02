// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealPortfolio : ModuleRules
{
	public UnrealPortfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateIncludePaths.AddRange(new string[] {
            "UnrealPortfolio",

            "UnrealPortfolio/AnimNotifyStates",

            "UnrealPortfolio/DataSave",

            "UnrealPortfolio/Commons/Characters",

            "UnrealPortfolio/EnviromentActors",

            "UnrealPortfolio/Enemy",
            "UnrealPortfolio/Enemy/AI",
            "UnrealPortfolio/Enemy/Boss",
            "UnrealPortfolio/Enemy/Projectiles",
            "UnrealPortfolio/Enemy/PatternObject",
            
            "UnrealPortfolio/Enemy/AI/Boss",
            "UnrealPortfolio/Enemy/AI/Boss/EnvQC",
            "UnrealPortfolio/Enemy/AI/Boss/BTTask",
            "UnrealPortfolio/Enemy/AI/Boss/BTService",

            "UnrealPortfolio/Enemy/AI/Enemy",
            "UnrealPortfolio/Enemy/AI/Enemy/BTTask",
            "UnrealPortfolio/Enemy/AI/Enemy/BTService",

            "UnrealPortfolio/GameModes",
            "UnrealPortfolio/GameInstances",

            "UnrealPortfolio/Level",

            "UnrealPortfolio/Orb",
            "UnrealPortfolio/Orb/Actors",
            "UnrealPortfolio/Orb/Struct",
            "UnrealPortfolio/Orb/Widget",

            "UnrealPortfolio/Player",
            "UnrealPortfolio/Player/CameraShake",

            "UnrealPortfolio/Struct",
            

            "UnrealPortfolio/Weapon",
            "UnrealPortfolio/Widget",
            "UnrealPortfolio/Widget/Store"
		});

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "AIModule", "GameplayTasks", "NavigationSystem", "Niagara", "MoviePlayer" });
    }
}
