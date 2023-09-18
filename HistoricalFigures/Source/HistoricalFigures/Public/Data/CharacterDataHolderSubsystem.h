// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharactersData.h"
#include "UObject/Object.h"
#include "CharacterDataHolderSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class HISTORICALFIGURES_API UCharacterDataHolderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="TCC | Data")
	FCharacterDescription SelectedCharacterDescription;
};
