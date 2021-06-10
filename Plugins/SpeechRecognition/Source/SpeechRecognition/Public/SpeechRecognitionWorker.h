#pragma once

#include <sphinxbase/err.h>
#include <sphinxbase/ad.h>
#include <pocketsphinx.h>
#include <stdio.h>
#include <time.h>
#include <regex>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <cstdio>
#include <vector>
#include <utility>

//General Log
DECLARE_LOG_CATEGORY_EXTERN(SpeechRecognitionPlugin, Log, All);

#define SENSCR_SHIFT 10

class  USpeechRecognitionComponent;

using namespace std;

//Common structures and enumerations
struct FSpeechRecognitionParam
{
	char* name;
	ESpeechRecognitionParamType type;
	char* value;

	// constructor
	FSpeechRecognitionParam(char* NewName, ESpeechRecognitionParamType NewType, char* NewValue) {
		name = new char[strlen(NewName) + 1];
		strcpy_s(name,sizeof(char[50]), NewName);
		type = NewType;
		value = new char[strlen(NewValue) + 1];
		strcpy_s(value, sizeof(char[50]), NewValue);
	}
};

class FSpeechRecognitionWorker :public FRunnable
{

private:
	// Sphinx
	ps_decoder_t* ps = NULL;
	cmd_ln_t* config = NULL;
	ad_rec_t* ad;
	int16 adbuf[2048];
	uint8 utt_started, in_speech;
	int32 k;
	bool initRequired = false;
	bool wordsAdded = false;

	//A set of params to apply to Sphinx initialisation
	TArray<FSpeechRecognitionParam> sphinxParams;

	//Speech detection mode
	ESpeechRecognitionMode detectionMode;

	//Thread
	FRunnableThread* Thread;

	//Pointer to our manager
	 USpeechRecognitionComponent* Manager;

	//Thread safe counter 
	FThreadSafeCounter StopTaskCounter;

	//Language
	const char* langStr = NULL;

	//Paths
	std::string contentPath_str;
	std::string argFilePath;
	std::string logPath;
	std::string modelPath;
	std::string languageModel;
	std::string dictionaryPath;

	//Stores the recognition keywords, along with their tolerances
	std::map <string, char*> keywords;

	//Dictionary
	std::map <string, set<string>> dictionary;

	//Splits a string by whitespace
	std::vector<std::string> Split(std::string s);

public:
	FSpeechRecognitionWorker();
	virtual ~FSpeechRecognitionWorker();

	//FRunnable interface
	virtual void Stop();
	virtual uint32 Run();

	//Methods to switch recognition modes
	bool EnableKeywordMode(TArray<FRecognitionPhrase> wordList);
	bool EnableGrammarMode(FString grammarName);

	//Action methods
	void AddWords(TArray<FRecognitionPhrase> dictionaryList);
	void InitConfig();
	bool SetConfigParam(FString param, ESpeechRecognitionParamType type, FString value);
	void SetLanguage(ESpeechRecognitionLanguage language);
	bool StartThread( USpeechRecognitionComponent* manager);
	void ShutDown();

	// Print Debug Text
	static void ClientMessage(FString txt);

};

