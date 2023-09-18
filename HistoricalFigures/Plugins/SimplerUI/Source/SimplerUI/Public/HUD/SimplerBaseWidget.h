// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplerActivatableWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/SizeBox.h"
#include "SimplerBaseWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SIMPLERUI_API USimplerBaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetBase(USimplerActivatableWidget* Base);
	void SetPrompt(USimplerActivatableWidget* Prompt);
	
protected:

	UPROPERTY(meta=(BindWidget))
	USizeBox* BasePanel;

	UPROPERTY(meta=(BindWidget))
	USizeBox* PromptPanel;

private:
	UFUNCTION()
	void RemoveWidgetFromBase();
	UPROPERTY()
	USimplerActivatableWidget* _widgetToActivate;

};
