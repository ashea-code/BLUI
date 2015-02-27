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

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Linux))
        {
            isLibrarySupported = true;


            /** //// Start Windows platform //// **/

            if ((Target.Platform == UnrealTargetPlatform.Win64))
            {
                PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Win/lib", "libcef.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Win/lib", "libcef_dll_wrapper.lib"));

                PublicIncludePaths.AddRange(
                    new string[] {
				        Path.Combine(ThirdPartyPath, "cef/Win"),
                        Path.Combine(ModulePath, "Blu/Public")
			    });
            }

            /** //// End Windows platform //// **/


            /** //// Start Linux 64 platform //// **/

            else if ((Target.Platform == UnrealTargetPlatform.Linux))
            {

                PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Linux/lib", "libcef.so"));
                PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef/Linux/lib", "libcef_dll_wrapper.a"));

                PublicIncludePaths.AddRange(
                    new string[] {
				        Path.Combine(ThirdPartyPath, "cef/Linux"),
                        Path.Combine(ModulePath, "Blu/Public")
			    });

            }

            /** //// End Linux 64 platform //// **/


            /** //// General Libs + Includes //// **/

            PrivateIncludePaths.AddRange(
            new string[] {
				Path.Combine(ModulePath, "Private")
			});

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
			});

        }

        Definitions.Add(string.Format("WITH_BLUI_BINDING={0}", isLibrarySupported ? 1 : 0));


	}

}