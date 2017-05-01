using System.Collections.Generic;
using UnrealBuildTool;
using System.IO;
using System;

public class Blu : ModuleRules
{

	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
	}

	private void stageFiles(String[] filesToStage)
	{
		foreach (var f in filesToStage)
		{
			RuntimeDependencies.Add(new RuntimeDependency(f));
		}
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

            PublicDelayLoadDLLs.Add("d3dcompiler_43.dll");
            PublicDelayLoadDLLs.Add("d3dcompiler_47.dll");
            PublicDelayLoadDLLs.Add("ffmpegsumo.dll");
            PublicDelayLoadDLLs.Add("libcef.dll");
            PublicDelayLoadDLLs.Add("libEGL.dll");
            PublicDelayLoadDLLs.Add("libGLESv2.dll");

			PublicIncludePaths.AddRange(
				new string[] {
					Path.Combine(ThirdPartyPath, "cef/Win")
				});

			// Add our runtime dependencies
			var filesToStage = Directory.GetFiles(Path.Combine(ThirdPartyPath, "cef/Win/shipping"), "*", SearchOption.AllDirectories);
			stageFiles(filesToStage);

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
			
			var frameworkPath = Path.Combine(ThirdPartyPath, "cef/Mac/lib", "Chromium Embedded Framework.framework");

			PublicFrameworks.Add(frameworkPath);
			PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Mac/lib", "libcef_dll_wrapper.a"));

			PublicIncludePaths.AddRange(
				new string[] {
					Path.Combine(ThirdPartyPath, "cef", "Mac")
				});

			var filesToStage = Directory.GetFiles(Path.Combine(ThirdPartyPath, "cef/Mac/shipping"), "*", SearchOption.AllDirectories);
			stageFiles(filesToStage);

			filesToStage = Directory.GetFiles(Path.Combine(ThirdPartyPath, "cef/Mac/lib"), "*", SearchOption.AllDirectories);
			stageFiles(filesToStage);

			if(!UEBuildConfiguration.bBuildEditor)
			{
				AdditionalBundleResources.Add(new UEBuildBundleResource(Path.Combine(frameworkPath, "Chromium Embedded Framework"), "MacOS", false));
			}

		}
		else
		{
			throw new BuildException("BLUI: Platform not supported");
		}
	}
}
