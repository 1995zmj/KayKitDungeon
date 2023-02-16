// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "KayAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UKayAssetManager : public UAssetManager
{
	GENERATED_BODY()


public:
	static const FPrimaryAssetType	PotionItemType;
	static const FPrimaryAssetType	SkillItemType;
	static const FPrimaryAssetType	TokenItemType;
	static const FPrimaryAssetType	WeaponItemType;

	virtual void StartInitialLoading() override;

};
