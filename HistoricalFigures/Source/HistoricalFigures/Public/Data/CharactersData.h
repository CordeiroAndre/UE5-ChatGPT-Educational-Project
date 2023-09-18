#pragma once
#include "Engine/DataTable.h"
#include "CharactersData.generated.h"




USTRUCT(BlueprintType)
struct FCharacterDescription
{
	GENERATED_BODY();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Occupation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Portrait;

	/*
	 * Algo como: voce eh o....
	 * */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CharacterProfile;

	/*
	 * Algo como: voce fez... voce criou
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CharacterBackground;

	/*
	 * Algo como: Me responda de tal maneira... Interaja coming de tal maneira...
	 * 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Request;

	
};

UCLASS(BlueprintType)
class HISTORICALFIGURES_API UCharactersDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCharacterDescription> Characters;
 
};