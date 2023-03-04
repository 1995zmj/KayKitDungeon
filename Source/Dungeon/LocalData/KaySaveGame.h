// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dungeon/Common/KayTypes.h"
#include "GameFramework/SaveGame.h"
#include "KaySaveGame.generated.h"

namespace EKaySaveGameVersion
{
	enum type_info
	{
		Initial,

		// -----<new versions must be added before this line>-------------------------------------------------
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};
}
UCLASS()
class DUNGEON_API UKaySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UKaySaveGame()
	{
		SaveDataVersion = EKaySaveGameVersion::LatestVersion;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FKayItemSlot, FPrimaryAssetId> SlottedItems;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;
	
protected:
	UPROPERTY()
	int32 SaveDataVersion;
};
