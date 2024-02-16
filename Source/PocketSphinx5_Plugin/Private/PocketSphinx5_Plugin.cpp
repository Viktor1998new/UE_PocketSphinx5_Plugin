// Copyright Epic Games, Inc. All Rights Reserved.

#include "PocketSphinx5_Plugin.h"

#define LOCTEXT_NAMESPACE "FPocketSphinx5_PluginModule"
DEFINE_LOG_CATEGORY(LogSphinxAPI)

void FPocketSphinx5_PluginModule::StartupModule()
{
    FString PluginPath;

    if (FPaths::DirectoryExists(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("PocketSphinx5_Plugin")))) {
        PluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("PocketSphinx5_Plugin"));
    }
    else if (FPaths::DirectoryExists(FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("Marketplace"), TEXT("PocketSphinx5_Plugin"))))
    {
        PluginPath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("Marketplace"), TEXT("PocketSphinx5_Plugin"));
    }

    FString dllPath = FPaths::Combine(PluginPath, TEXT("ThirdParty"));

    if (FPaths::FileExists(FPaths::Combine(dllPath, TEXT("pocketsphinx.dll"))))
    {
        FPlatformProcess::AddDllDirectory(*FPaths::Combine(dllPath, TEXT("pocketsphinx.dll"))); // only load dll when needed for use. Broken with 4.11.
        FPlatformProcess::GetDllHandle(*FPaths::Combine(dllPath, TEXT("pocketsphinx.dll"))); // auto-load dll with plugin - needed as 4.11 breaks above line.
        UE_LOG(LogTemp, Warning, TEXT("Load Lib pocketsphinx : %s"), *FPaths::Combine(dllPath, TEXT("pocketsphinx.dll")));
    }
    else {
        checkf(false, TEXT("Failed to load pocketsphinx dll"));
    }
}

void FPocketSphinx5_PluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPocketSphinx5_PluginModule, PocketSphinx5_Plugin)