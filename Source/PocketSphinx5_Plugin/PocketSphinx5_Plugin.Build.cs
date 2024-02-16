// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class PocketSphinx5_Plugin : ModuleRules
{

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
    }

    public PocketSphinx5_Plugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "AudioCaptureCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "AudioCaptureCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);


        PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "pocketsphinx.lib"));

        // TODO: Copy dlls to alternative package directory, to be loaded through a manual process
        PublicDelayLoadDLLs.Add("pocketsphinx.dll");
        RuntimeDependencies.Add(Path.Combine(ThirdPartyPath, "pocketsphinx.dll"));
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "include"));
    }
}
