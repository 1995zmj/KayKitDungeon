// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dungeon/Asset/KayDataAsset.h"
#include "Dungeon/Common/KayInventoryInterface.h"
#include "Dungeon/Common/KayTypes.h"
#include "KayPlayerController.generated.h"


UCLASS()
class DUNGEON_API AKayPlayerController : public APlayerController, public IKayInventoryInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKayPlayerController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<UKayDataAsset*, FKayItemData> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FKayItemSlot, UKayDataAsset*> SlottedItems;

	UPROPERTY(BlueprintAssignable, Category = "Inventroy")
	FOnInventoryItemChanged OnInventoryItemChanged; 

	FOnInventoryItemChangedNative OnInventoryItemChangedNative;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnSlottedItemChanged OnSlottedItemChanged;

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void InventoryItemChanged(bool bAdded, UKayDataAsset* Item);

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventroy")
	void SlottedItemChanged(FKayItemSlot ItemSlot, UKayDataAsset* Item);

	FOnSlottedItemChangedNative OnSlottedItemChangedNative;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryLoaded OnInventoryLoaded;

	FOnInventoryLoadedNative OnInventoryLoadedNative;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddInventoryItem(UKayDataAsset* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveInventoryItem(UKayDataAsset* RemovedItem, int32 RemoveCount = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetInventoryItems(TArray<UKayDataAsset*>& Items, FPrimaryAssetType ItemType);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetInventoryItemCount(UKayDataAsset* Item) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool GetInventoryItemData(UKayDataAsset* Item, FKayItemData& ItemData) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool SetSlottedItem(FKayItemSlot ItemSlot, UKayDataAsset* Item);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UKayDataAsset* GetSlottedItem(FKayItemSlot ItemSlot) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GetSlottedItems(TArray<UKayDataAsset*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void FillEmptySlots();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SaveInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool LoadInventory();

	virtual const TMap<UKayDataAsset*, FKayItemData>& GetInventoryDataMap() const override
	{
		return InventoryData;
	}
	
	virtual const TMap<FKayItemSlot, UKayDataAsset*>& GetSlottedItemMap() const override
	{
		return SlottedItems;
	}
	
	virtual FOnInventoryItemChangedNative& GetInventoryItemChangedDelegate() override
	{
		return OnInventoryItemChangedNative;
	}
	
	virtual FOnSlottedItemChangedNative& GetSlottedItemChangedDelegate() override
	{
		return OnSlottedItemChangedNative;
	}
	
	virtual FOnInventoryLoadedNative& GetInventoryLoadedDelegate() override
	{
		return OnInventoryLoadedNative;
	}

protected:
	bool FillEmptySlotWithItem(UKayDataAsset* NewItem);

	void NotifyInventoryItemChanged(bool bAdded, UKayDataAsset* Item);
	void NotifySlottedItemChanged(FKayItemSlot ItemSlot, UKayDataAsset* Item);
	void NotifyInventoryLoaded();

	void HandleSaveGameLoaded(UKaySaveGame* NewSaveGame);
	
};
