// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KayDataAsset.h"
#include "KayDataAsset_Skill.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UKayDataAsset_Skill : public UKayDataAsset
{
	GENERATED_BODY()

public:
	UKayDataAsset_Skill()
	{
		ItemType = UKayAssetManager::SkillItemType;
	}
	
};
