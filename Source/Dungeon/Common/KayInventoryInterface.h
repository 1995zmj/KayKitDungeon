// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KayTypes.h"
#include "Dungeon/Asset/KayDataAsset.h"
#include "UObject/Interface.h"
#include "KayInventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UKayInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEON_API IKayInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual const TMap<UKayDataAsset*, FKayItemData>& GetInventoryDataMap() const = 0;
	
	virtual const TMap<FKayItemSlot, UKayDataAsset*>& GetSlottedItemMap() const = 0;
	
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() = 0;
	
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() = 0;
	
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() = 0;
	
};
