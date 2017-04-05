// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

namespace UnrealBuildTool.Rules
{
	public class OggAsset : ModuleRules
	{
		public OggAsset(TargetInfo Target)
		{
            PublicDependencyModuleNames.AddRange(
                new string[] {
                    "Engine",
					"Core",
					"CoreUObject",
                }
            );

			PrivateIncludePaths.AddRange(
				new string[] {
					"Runtime/OggAsset/Private",
				}
			);
		}
	}
}
