// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KayTypes.generated.h"


USTRUCT(BlueprintType)
struct DUNGEON_API FKayItemSlot
{
	GENERATED_BODY()

	/** Constructor, -1 means an invalid slot */
	FKayItemSlot()
		: SlotNumber(-1)
	{}

	FKayItemSlot(const FPrimaryAssetType& InItemType, int32 InSlotNumber)
		: ItemType(InItemType)
		, SlotNumber(InSlotNumber)
	{}

	/** The type of items that can go in this slot */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FPrimaryAssetType ItemType;

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotNumber;

	/** Equality operators */
	bool operator==(const FKayItemSlot& Other) const
	{
		return ItemType == Other.ItemType && SlotNumber == Other.SlotNumber;
	}
	bool operator!=(const FKayItemSlot& Other) const
	{
		return !(*this == Other);
	}
	
	/** Implemented so it can be used in Maps/Sets */
	friend inline uint32 GetTypeHash(const FKayItemSlot& Key)
	{
		uint32 Hash = 0;
	
		Hash = HashCombine(Hash, GetTypeHash(Key.ItemType));
		Hash = HashCombine(Hash, (uint32)Key.SlotNumber);
		return Hash;
	}

	/** Returns true if slot is valid */
	bool IsValid() const
	{
		return ItemType.IsValid() && SlotNumber >= 0;
	}
};
