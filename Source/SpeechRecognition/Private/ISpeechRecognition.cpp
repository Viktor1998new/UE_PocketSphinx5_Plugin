// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ISpeechRecognition.h"

IMPLEMENT_MODULE(FSpeechRecognition, SpeechRecognition)


void FSpeechRecognition::StartupModule()
{
    if (PLATFORM_WINDOWS) {

        FString PluginPath;

        if (FPaths::DirectoryExists(FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("SpeechRecognition")))) {
            PluginPath = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("SpeechRecognition"));
        }
        else if (FPaths::DirectoryExists(FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("SpeechRecognition"))))
        {
            PluginPath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("SpeechRecognition"));
        }

        UE_LOG(LogTemp, Warning, TEXT("PluginPath: %s"), *FPaths::Combine(FPaths::ProjectPluginsDir(), *PluginPath));

        FString LibPocketSphinx = FPaths::Combine(PluginPath, TEXT("ThirdParty"), TEXT("PocketSphinx"), TEXT("Libraries"));


#if WIN32
        LibPocketSphinx = FPaths::Combine(LibPocketSphinx, TEXT("x32"));
#else
        LibPocketSphinx = FPaths::Combine(LibPocketSphinx, TEXT("x64"));
#endif

        if (FPaths::FileExists(FPaths::Combine(LibPocketSphinx, TEXT("pocketsphinx.dll"))))
        {
            FPlatformProcess::AddDllDirectory(*FPaths::Combine(LibPocketSphinx, TEXT("pocketsphinx.dll"))); // only load dll when needed for use. Broken with 4.11.
            FPlatformProcess::GetDllHandle(*FPaths::Combine(LibPocketSphinx, TEXT("pocketsphinx.dll"))); // auto-load dll with plugin - needed as 4.11 breaks above line.
            UE_LOG(LogTemp, Warning, TEXT("Load Lib PocketSphinx: %s"), *FPaths::Combine(LibPocketSphinx, TEXT("pocketsphinx.dll")));
        }
        else {
            checkf(false, TEXT("Failed to load PocketSphinx dll"));
        }

        FString LibSphinxBase = FPaths::Combine(PluginPath, TEXT("ThirdParty"), TEXT("SphinxBase"), TEXT("Libraries"));


#if WIN32
        LibSphinxBase = FPaths::Combine(LibSphinxBase, TEXT("x32"));
#else
        LibSphinxBase = FPaths::Combine(LibSphinxBase, TEXT("x64"));
#endif

        if (FPaths::FileExists(FPaths::Combine(LibSphinxBase, TEXT("sphinxbase.dll"))))
        {
            FPlatformProcess::AddDllDirectory(*FPaths::Combine(LibPocketSphinx, TEXT("sphinxbase.dll"))); // only load dll when needed for use. Broken with 4.11.
            FPlatformProcess::GetDllHandle(*FPaths::Combine(LibPocketSphinx, TEXT("sphinxbase.dll"))); // auto-load dll with plugin - needed as 4.11 breaks above line.
            UE_LOG(LogTemp, Warning, TEXT("Load Lib SphinxBase : %s"), *FPaths::Combine(LibSphinxBase, TEXT("sphinxbase.dll")));
        }
        else {
            checkf(false, TEXT("Failed to load SphinxBase dll"));
        }
    }
}

void FSpeechRecognition::ShutdownModule()
{
    //TODO
}
