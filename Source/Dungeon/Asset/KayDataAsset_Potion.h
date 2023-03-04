// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KayDataAsset.h"
#include "KayDataAsset_Potion.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UKayDataAsset_Potion : public UKayDataAsset
{
	GENERATED_BODY()

public:

	UKayDataAsset_Potion()
	{
		ItemType = UKayAssetManager::PotionItemType;
	}
};
