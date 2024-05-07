// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LASTLASTTEST : ModuleRules
{
	public LASTLASTTEST(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "HTTP", "Json", "JsonUtilities" });
	}
}
