// Fill out your copyright notice in the Description page of Project Settings.


#include "KayAssetManager.h"
#include "Dungeon/Tool/KayLog.h"
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

UKayAssetManager& UKayAssetManager::Get()
{
	UKayAssetManager* This = Cast<UKayAssetManager>(GEngine->AssetManager);

	if (This)
	{
		return *This;
	}
	else
	{
		UE_LOG(LogKay, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be RPGAssetManager!"));
		return *NewObject<UKayAssetManager>();
	}
}

UKayDataAsset* UKayAssetManager::ForceLoadItem(const FPrimaryAssetId& PrimaryAssetId, bool bLogWarning)
{
	const FSoftObjectPath ItemPath = GetPrimaryAssetPath(PrimaryAssetId);

	UKayDataAsset* LoadedItem = Cast<UKayDataAsset>(ItemPath.TryLoad());

	if (bLogWarning && LoadedItem == nullptr)
	{
		UE_LOG(LogKay, Warning, TEXT("Failed to load item for identifier %s!"), *PrimaryAssetId.ToString());
	}

	return LoadedItem;
}


