// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KayDataAsset.h"
#include "Dungeon/Equipment/KayWeapon.h"
#include "Dungeon/Manager/KayAssetManager.h"
#include "KayDataAsset_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UKayDataAsset_Weapon : public UKayDataAsset
{
	GENERATED_BODY()
public:
	UKayDataAsset_Weapon()
	{
		ItemType = UKayAssetManager::WeaponItemType;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AKayWeapon> WeaponActor;
};
