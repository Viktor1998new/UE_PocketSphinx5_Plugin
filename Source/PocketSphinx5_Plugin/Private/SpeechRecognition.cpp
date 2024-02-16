#include "SpeechRecognition.h"
#include "PocketSphinx5_Plugin.h"
#include "AudioCaptureCore.h"

uint32 SpeechRecognitionThread::Run()
{
    Audio::FAudioCaptureSynth CaptureSynth;

    CaptureSynth.OpenDefaultStream();

    Audio::FCaptureDeviceInfo DeviceInfo;

    bool IsAudio = CaptureSynth.GetDefaultCaptureDeviceInfo(DeviceInfo) && (DeviceInfo.InputChannels == 1 || DeviceInfo.InputChannels == 2);

    if (!IsAudio) {
        UE_LOG(LogAudio, Warning, TEXT("Audio capture components only support mono and stereo mic input."));
        return 1;
    }

    CaptureSynth.StartCapturing();

    size_t frame_size = ps_endpointer_frame_size(ep);
    int16* frame{};

    if ((frame = (int16*)malloc(frame_size * sizeof(frame[0]))) == NULL) {
        UE_LOG(LogSphinxAPI, Error, TEXT("Failed to allocate frame"));
        return 1;
    }

    ps_vad_t*  in_speech = ps_vad_init(ps_vad_mode_t::PS_VAD_LOOSE, DeviceInfo.PreferredSampleRate, frame_size);
    ps_start_utt(decoder);

    UE_LOG(LogSphinxAPI, Log, TEXT("Begin"));

    while (true)
    {
        TArray<float> CaptureAudioData;
        CaptureAudioData.Reserve(DeviceInfo.PreferredSampleRate);
        CaptureSynth.GetAudioData(CaptureAudioData);

        TArray<int16> Int16AudioData;
        Int16AudioData.Reserve(CaptureAudioData.Num());

        for (float Value : CaptureAudioData)
        {
            int16 IntValue = static_cast<int16>(Value * 32767);
            Int16AudioData.Add(IntValue);
        }

        frame = Int16AudioData.GetData();
            
        if (!frame)
            continue;

        
        int prev_in_speech = ps_endpointer_in_speech(ep);
        const int16* speech = ps_endpointer_process(ep, frame);

        if (speech != NULL) {
            const char* hyp;
                
            if (!prev_in_speech)
                ps_start_utt(decoder);
                
            if (ps_process_raw(decoder, speech, frame_size, FALSE, FALSE) < 0)
                UE_LOG(LogSphinxAPI, Log, TEXT("ps_process_raw() failed"));
               
            if (!ps_endpointer_in_speech(ep)) {
                ps_end_utt(decoder);
                if ((hyp = ps_get_hyp(decoder, NULL)) != NULL) {
                    OnListening.Broadcast(UTF8_TO_TCHAR(hyp));
                }
            }
        }
    }
   
    return 0;
}

void USpeechRecognitionComponent::BeginDestroy()
{
    Free();
}

void USpeechRecognitionComponent::Listen()
{
    if (Thread != nullptr)
        return;

    RunnableThread = MakeUnique<SpeechRecognitionThread>();
    RunnableThread->config = config;
    RunnableThread->decoder = decoder;
    RunnableThread->ep = ep;
    RunnableThread->OnListening = OnListening;

    Thread = FRunnableThread::Create(RunnableThread.Get(), TEXT("SpeechRecognition"));
}

void USpeechRecognitionComponent::UnListen() {

    if (Thread == nullptr)
        return;

    delete Thread;
    Thread = nullptr;
}

void USpeechRecognitionComponent::Free()
{
    UnListen();
    ps_free(decoder);
    ps_config_free(config);
}

bool USpeechRecognitionComponent::InitDecoder() {

    if ((decoder = ps_init(config)) == NULL) {
        UE_LOG(LogSphinxAPI, Error, TEXT("PocketSphinx decoder init failed"));
        return false;
    }
    return true;

}
bool USpeechRecognitionComponent::InitEndpointer() {

    if ((ep = ps_endpointer_init(0, 0.0, PS_VAD_LOOSE, 0, 0)) == NULL) {
        UE_LOG(LogSphinxAPI, Error, TEXT("PocketSphinx endpointer init failed"));
        return false;
    }
    return true;
}

bool USpeechRecognitionComponent::InitConfig()
{
    config = ps_config_init(NULL);
    ps_default_search_args(config);

    return true;
}

USpeechRecognitionComponent::~USpeechRecognitionComponent()
{
    Free();
}

FString USpeechRecognitionComponent::GetJsonConfig() {

    return UTF8_TO_TCHAR(ps_config_serialize_json(config));
}

void USpeechRecognitionComponent::SetConfigInt(FString Name, int NewValue)
{
    if (config != nullptr)
        ps_config_set_int(config, TCHAR_TO_ANSI(*Name), NewValue);
}

void USpeechRecognitionComponent::SetConfigBool(FString Name, bool NewValue)
{
    if (config != nullptr)
        ps_config_set_bool(config, TCHAR_TO_ANSI(*Name), NewValue);
}

void USpeechRecognitionComponent::SetConfigFloat(FString Name, float NewValue)
{
    if (config != nullptr)
        ps_config_set_float(config, TCHAR_TO_ANSI(*Name), NewValue);
}

void USpeechRecognitionComponent::SetConfigString(FString Name, FString NewValue)
{
    if(config != nullptr)
        ps_config_set_str(config, TCHAR_TO_ANSI(*Name), TCHAR_TO_ANSI(*NewValue));
}

int USpeechRecognitionComponent::GetConfigInt(FString Name)
{
    if (config != nullptr)
        return ps_config_int(config, TCHAR_TO_ANSI(*Name));

    return 0;
}

bool USpeechRecognitionComponent::GetConfigBool(FString Name)
{
    if (config != nullptr)
        return (bool)ps_config_bool(config, TCHAR_TO_ANSI(*Name));

    return false;
}
 
float USpeechRecognitionComponent::GetConfigFloat(FString Name)
{
    if (config != nullptr)
        return ps_config_float(config, TCHAR_TO_ANSI(*Name));

    return .0f;
}

FString USpeechRecognitionComponent::GetConfigString(FString Name)
{
    if (config != nullptr)
        return UTF8_TO_TCHAR(ps_config_str(config, TCHAR_TO_ANSI(*Name)));

    return "";
}
