// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class SpeechRecognition : ModuleRules
{

    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public SpeechRecognition(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
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
                "RHI"
				// ... add private dependencies that you statically link with here ...	
			}
			);

        LoadSphinxBase(Target);
        LoadPocketSphinx(Target);
    }


    public bool LoadSphinxBase(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x32";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "SphinxBase", "Libraries");
            LibrariesPath = Path.Combine(LibrariesPath, PlatformString);

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "sphinxbase.lib"));

            // TODO: Copy dlls to alternative package directory, to be loaded through a manual process
            PublicDelayLoadDLLs.Add("sphinxbase.dll");
            RuntimeDependencies.Add(Path.Combine(LibrariesPath, "sphinxbase.dll"));
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "SphinxBase", "include"));
            PrivateIncludePaths.Add(Path.Combine(ThirdPartyPath, "SphinxBase", "include"));
        }

        PublicDefinitions.Add(string.Format("WITH_SPHINX_BASE_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }

    public bool LoadPocketSphinx(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x32";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "PocketSphinx", "Libraries");
            LibrariesPath = Path.Combine(LibrariesPath, PlatformString);

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "pocketsphinx.lib"));

            // TODO: Copy dlls to alternative package directory, to be loaded through a manual process
            PublicDelayLoadDLLs.Add("pocketsphinx.dll");
            RuntimeDependencies.Add(Path.Combine(LibrariesPath, "pocketsphinx.dll"));
        }


        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "PocketSphinx", "include"));
            PrivateIncludePaths.Add(Path.Combine(ThirdPartyPath, "PocketSphinx", "include"));
        }

        PublicDefinitions.Add(string.Format("WITH_POCKET_SPHINX_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
