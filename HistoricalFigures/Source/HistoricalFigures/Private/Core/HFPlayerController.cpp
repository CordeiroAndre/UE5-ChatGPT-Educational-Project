// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HFPlayerController.h"
#include "SpeechRecognizer.h"
#include "JsonObjectConverter.h"
#include "Core/HFGPT3Request.h"
#include "Core/Models/HFGPTModel.h"
#include "Data/CharacterDataHolderSubsystem.h"
#include "Sound/CapturableSoundWave.h"


void AHFPlayerController::StartCapturingMic()
{
	if(CanCaptureMic())
	{
		CurrentRecognizedText.Empty();
		ChangeStatus(EUserStatus::CAPTURING);

		/* Create the object that will convert the speech to text and bind its delegates */
		SpeechRecognizer = USpeechRecognizer::CreateSpeechRecognizer();
		SpeechRecognizer->OnRecognitionFinished.AddDynamic(this, &AHFPlayerController::HandleRecognitionFinished);
		SpeechRecognizer->OnRecognizedTextSegment.AddDynamic(this, &AHFPlayerController::HandleRecognizedTextSegment);

		/* Configure the speech recognition */
		SpeechRecognizer->SetStreamingDefaults();
		SpeechRecognizer->SetLanguage(ESpeechRecognizerLanguage::Pt);
		SpeechRecognizer->SetNumOfThreads(4);

		/* Enable the speech recognizer to receive audio */
		SpeechRecognizer->StartSpeechRecognition(FOnSpeechRecognitionStartedDynamic());

		/* Create the object that will capture our mic */
		CapturableSoundWave =  UCapturableSoundWave::CreateCapturableSoundWave();
		CapturableSoundWave->OnPopulateAudioData.AddDynamic(this, &AHFPlayerController::HandlePopulatedAudioData);

		/*Start capturing the */
		CapturableSoundWave->StartCapture(0);
	}
}

void AHFPlayerController::StopCapturingMic()
{
	if(CurrentStatus==EUserStatus::CAPTURING)
	{
		ChangeStatus(EUserStatus::TRANSFORMING_SPEECH_TO_TEXT);
		SpeechRecognizer->ForceProcessPendingAudioData();
		CapturableSoundWave->StopCapture();
	}
}

void AHFPlayerController::HandleReceivedResponseFromGPT3(const FString& Response)
{
	if(!Response.IsEmpty())
	{
		AddChatEntry("assistant", Response);
	}
	ChangeStatus(EUserStatus::FINISHED);
}

void AHFPlayerController::ConfirmSpeechRecognition(const bool& Accepted)
{
	if(Accepted)
	{
		FRequestModel RequestModel;
		TArray<FMessageModel> AuxMessageContainer;

		AddChatEntry("user",CurrentRecognizedText);
		
		for (TTuple<FString, FString> Message : Chat)
		{
			FMessageModel RequestMessages;
			RequestMessages.role = Message.Key;
			RequestMessages.content = Message.Value;
			AuxMessageContainer.Add(RequestMessages);
		}
		RequestModel.messages = AuxMessageContainer;
		FString JSonString;
		FJsonObjectConverter::UStructToJsonObjectString(FRequestModel::StaticStruct(),&RequestModel,JSonString);

		
		UHFGPT3Request::PostMessageToChatGPT3(OpenAIAPIKey, JSonString)->OnResponse.AddDynamic(this, &AHFPlayerController::HandleReceivedResponseFromGPT3);
		
		ChangeStatus(EUserStatus::GENERATING_ANSWER);
	}
	else
	{
		ChangeStatus(EUserStatus::FINISHED);
	}
}

bool AHFPlayerController::CanCaptureMic()
{
	if(CurrentStatus==EUserStatus::TRANSFORMING_SPEECH_TO_TEXT) return false;
	if(CurrentStatus==EUserStatus::GENERATING_ANSWER) return false;
	return true;
}

void AHFPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto CharacterDescription = GetGameInstance()->GetSubsystem<UCharacterDataHolderSubsystem>()->SelectedCharacterDescription;
	FString SysContextAsString;

	SysContextAsString.Append(CharacterDescription.CharacterProfile);
	SysContextAsString.Append(CharacterDescription.CharacterBackground);
	SysContextAsString.Append(CharacterDescription.Request);

	SystemContext = SysContextAsString;
	AddChatEntry("system", SystemContext);
}

void AHFPlayerController::HandleRecognitionFinished()
{
	if(CapturableSoundWave->IsCapturing())
	{
		SpeechRecognizer->StopSpeechRecognition();
		SpeechRecognizer = nullptr;
	}
}

void AHFPlayerController::HandleRecognizedTextSegment(const FString& RecognizedWords)
{
	CurrentRecognizedText += RecognizedWords;
	OnCapturedMic.Broadcast(CurrentRecognizedText);
	ChangeStatus(EUserStatus::FINISHED);
}

void AHFPlayerController::HandlePopulatedAudioData(const TArray<float>& PopulatedAudioData)
{
	SpeechRecognizer->ProcessAudioData(PopulatedAudioData, CapturableSoundWave->GetSampleRate(), CapturableSoundWave->GetNumOfChannels(), false);
}
