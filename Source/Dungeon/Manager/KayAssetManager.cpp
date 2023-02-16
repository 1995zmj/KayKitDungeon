// Fill out your copyright notice in the Description page of Project Settings.


#include "KayAssetManager.h"

#include "Dungeon/Asset/KayDataAsset_Weapon.h"

const FPrimaryAssetType	UKayAssetManager::PotionItemType = TEXT("Potion");
const FPrimaryAssetType	UKayAssetManager::SkillItemType = TEXT("Skill");
const FPrimaryAssetType	UKayAssetManager::TokenItemType = TEXT("Token");
const FPrimaryAssetType	UKayAssetManager::WeaponItemType = TEXT("Weapon");

void UKayAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	ScanPathsForPrimaryAssets(UKayAssetManager::WeaponItemType,TArray<FString>{"/Game/Dungeon/Core/DataAsset"},UKayDataAsset_Weapon::StaticClass(),false);
}


