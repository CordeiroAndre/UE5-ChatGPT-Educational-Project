// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HFGPT3Request.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGotResponse, const FString&, AssistantResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGotResponseFail);

UCLASS()
class HISTORICALFIGURES_API UHFGPT3Request : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category="TCC | GPT3 | Requests")
	static UHFGPT3Request* PostMessageToChatGPT3(const FString& APIKey, const FString& JSonContentAsString);

	UPROPERTY(BlueprintAssignable, Category="TCC | GPT3 | Delegates")
	FOnGotResponse OnResponse;

	UPROPERTY(BlueprintAssignable, Category="TCC | GPT3 | Delegates")
	FOnGotResponseFail OnError;

private:
	void HandleRequestComplete(TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> HttpRequest, TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> HttpResponse, bool bSuccessful);

};
