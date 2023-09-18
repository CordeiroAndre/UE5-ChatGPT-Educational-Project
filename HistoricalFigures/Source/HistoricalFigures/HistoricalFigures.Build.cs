// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HistoricalFigures : ModuleRules
{
	public HistoricalFigures(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "RuntimeSpeechRecognizer", "HTTP", "JsonUtilities", "Json", "RuntimeAudioImporter"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

	}
}
