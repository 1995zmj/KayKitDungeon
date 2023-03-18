// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "KayAssetManager.generated.h"

class UKayDataAsset;
/**
 * 
 */
UCLASS()
class DUNGEON_API UKayAssetManager : public UAssetManager
{
	GENERATED_BODY()


public:


	virtual void StartInitialLoading() override;
	
	static const FPrimaryAssetType	PotionItemType;
	static const FPrimaryAssetType	SkillItemType;
	static const FPrimaryAssetType	TokenItemType;
	static const FPrimaryAssetType	WeaponItemType;

	static UKayAssetManager& Get();

	UKayDataAsset* ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning = true);
};
