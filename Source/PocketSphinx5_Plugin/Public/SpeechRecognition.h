// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include <pocketsphinx.h>
#include "Components/ActorComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SpeechRecognition.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnListening, const FString&, Text);

class SpeechRecognitionThread : public FRunnable
{
public:

    ps_decoder_t* decoder;
    ps_config_t* config;
    ps_endpointer_t* ep;
    FOnListening OnListening;

    uint32 Run() override;

};

UCLASS(ClassGroup = (Speech), meta = (BlueprintSpawnableComponent), NotBlueprintable, BlueprintType)
class POCKETSPHINX5_PLUGIN_API USpeechRecognitionComponent : public UActorComponent {

    GENERATED_BODY()

public:

    ps_decoder_t* decoder;
    ps_config_t* config;
    ps_endpointer_t* ep;

    TUniquePtr<SpeechRecognitionThread> RunnableThread;
    FRunnableThread* Thread;

    UPROPERTY(BlueprintAssignable)
        FOnListening OnListening;

        virtual void BeginDestroy() override;

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition")
        void Listen();

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition")
        void UnListen();

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition")
        void Free();

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition")
        bool InitDecoder();

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition")
        bool InitEndpointer();

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition")
        bool InitConfig();

    UFUNCTION(BlueprintPure, Category = "SpeechRecognition")
        FString GetJsonConfig();

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition|Config")
         void SetConfigInt(FString Name, int NewValue);

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition|Config")
         void SetConfigBool(FString Name, bool NewValue);

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition|Config")
         void SetConfigFloat(FString Name, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "SpeechRecognition|Config")
         void SetConfigString(FString Name, FString NewValue);
         
    UFUNCTION(BlueprintPure, Category = "SpeechRecognition|Config")
         int GetConfigInt(FString Name);

    UFUNCTION(BlueprintPure, Category = "SpeechRecognition|Config")
         bool GetConfigBool(FString Name);

    UFUNCTION(BlueprintPure, Category = "SpeechRecognition|Config")
         float GetConfigFloat(FString Name);

    UFUNCTION(BlueprintPure, Category = "SpeechRecognition|Config")
         FString GetConfigString(FString Name);

    ~USpeechRecognitionComponent();
};