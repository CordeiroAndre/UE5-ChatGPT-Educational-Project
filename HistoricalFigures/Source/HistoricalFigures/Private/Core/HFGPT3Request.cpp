// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HFGPT3Request.h"
#include "HttpModule.h"


void UHFGPT3Request::PostMessageToChatGPT3(const FString& APIKey, const FString& JSonContentAsString)
{
	
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
	
	FString Authorization = "Bearer ";
	Authorization.Append(APIKey);
	httpRequest->SetURL("https://api.openai.com/v1/chat/completions");
	httpRequest->SetVerb(TEXT("POST"));
	httpRequest->SetHeader("Content-Type", "application/json");
	httpRequest->SetHeader("Authorization", Authorization);
	httpRequest->SetContentAsString(JSonContentAsString);
	
	httpRequest->OnProcessRequestComplete().BindUObject(this, &UHFGPT3Request::HandleRequestComplete);
	httpRequest->ProcessRequest();
}

void UHFGPT3Request::HandleRequestComplete(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest,TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> HttpResponse, bool bSuccessful)
{
	if (bSuccessful)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(HttpResponse->GetContentAsString());
		if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
		{
			TArray<TSharedPtr<FJsonValue>> choices = JsonObject->GetArrayField("choices");
				
			if (choices.Num() > 0)
			{
				TSharedPtr<FJsonObject> messageObj = choices[0]->AsObject()->GetObjectField("message");
				
				if (messageObj.IsValid())
				{
					FString content = messageObj->GetStringField("content");
					this->OnResponse.Broadcast(content);
				}
			}
		}
	}
	else
	{
		this->OnError.Broadcast();
		UE_LOG(LogTemp, Error, TEXT("CURL request failed: %s"), *HttpResponse->GetContentAsString());
	}
}
