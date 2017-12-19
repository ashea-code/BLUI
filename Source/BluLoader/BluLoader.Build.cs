using UnrealBuildTool;
using System.IO;
using System;

public class BluLoader : ModuleRules
{
	public BluLoader(ReadOnlyTargetRules Target) : base(Target)
    {
		PublicDependencyModuleNames.AddRange(
			new string[]
		{
			"Core",
			"CoreUObject",
			"Engine"
		});
	}
}
