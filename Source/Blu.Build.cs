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
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../ThirdParty/")); }
    }

	public Blu(TargetInfo Target)
	{

        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            isLibrarySupported = true;

            PrivateIncludePaths.AddRange(
            new string[] {
				Path.Combine(ModulePath, "Private")
				// ... add other private include paths required here ...
			});

            PublicIncludePaths.AddRange(
            new string[] {
				Path.Combine(ThirdPartyPath, "cef/Win"),
                Path.Combine(ModulePath, "Blu/Public")
			});

            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Win/lib", "libcef.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Win/lib", "libcef_dll_wrapper.lib"));

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
                "Json", 
                "JsonUtilities"
				// ... add other public dependencies that you statically link with here ...
			});

        }

        Definitions.Add(string.Format("WITH_BLUI_BINDING={0}", isLibrarySupported ? 1 : 0));


	}

}