using UnrealBuildTool;
using System.IO;
using System;

public class Blu : ModuleRules
{

    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

	public Blu(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"RenderCore",
			"RHI",
			"Slate",
			"SlateCore",
			"UMG",
			"Json"
		});

		PrivateIncludePaths.AddRange(
			new string[] {
				"Blu/Private",
			});

	

		if(Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Win/lib", "libcef.lib"));
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Win/lib", "libcef_dll_wrapper.lib"));

			PublicIncludePaths.AddRange(
				new string[] {
					Path.Combine(ThirdPartyPath, "cef/Win")
				});
		} else if(Target.Platform == UnrealTargetPlatform.Linux)
		{

			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Linux/lib", "libcef.so"));
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Linux/lib", "libcef_dll_wrapper.a"));

			PublicIncludePaths.AddRange(
				new string[] {
					Path.Combine(ThirdPartyPath, "cef/Linux")
				});
		} else if(Target.Platform == UnrealTargetPlatform.Mac)
		{
			string Configuration;
			switch(Target.Configuration)
			{
			case UnrealTargetConfiguration.Debug:
				Configuration = "Debug";
				break;
			
			case UnrealTargetConfiguration.DebugGame:
				Configuration = "Debug";
				break;

			default:
				Configuration = "Release";
				break;
			}
				
			PublicFrameworks.Add(Path.Combine(ThirdPartyPath, "cef", "Mac", Configuration, "Chromium Embedded Framework.framework"));
			
			PublicIncludePaths.AddRange(
				new string[] {
					Path.Combine(ThirdPartyPath, "cef", "Mac")
				});
		}
		else
		{
			throw new BuildException("BLUI: Platform not supported");
		}
	}
}