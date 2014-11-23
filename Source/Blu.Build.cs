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
		PrivateIncludePaths.AddRange(
			new string[] {
				Path.Combine(ModulePath, "Private")
				// ... add other private include paths required here ...
			});

        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef\\Win\\Release", "libcef.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "cef\\Win\\Release", "libcef_dll_wrapper.lib"));
        PrivateIncludePaths.Add(Path.Combine(ThirdPartyPath, "cef\\Win"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "cef\\Win"));


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"RenderCore",
				"RHI"
				// ... add other public dependencies that you statically link with here ...
			});
	}

}