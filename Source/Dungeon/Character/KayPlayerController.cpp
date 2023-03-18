// Fill out your copyright notice in the Description page of Project Settings.


#include "KayPlayerController.h"

#include "Dungeon/LocalData/KaySaveGame.h"
#include "Dungeon/Subsystem/KayGameInstanceSubsystem.h"
#include "Dungeon/Tool/KayLog.h"


// Sets default values
AKayPlayerController::AKayPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKayPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKayPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AKayPlayerController::AddInventoryItem(UKayDataAsset* NewItem, int32 ItemCount, int32 ItemLevel, bool bAutoSlot)
{
	bool bChanged = false;

	if (!NewItem)
	{
		UE_LOG(LogKay, Warning, TEXT("AddInventoryItem: Failed trying to add null item!"));
		return false;
	}
	
	if (ItemCount <= 0 || ItemLevel <= 0)
    {
    	UE_LOG(LogKay, Warning, TEXT("AddInventoryItem: Failed trying to add item %s with negative count or level!"), *NewItem->GetName());
    	return false;
    }

	FKayItemData OldData;
	GetInventoryItemData(NewItem, OldData);

	FKayItemData NewData = OldData;

	NewData.UpdateItemData(FKayItemData(ItemCount, ItemLevel), NewItem->MaxCount, NewItem->MaxLevel);

	if (OldData != NewData)
	{
		InventoryData.Add(NewItem, NewData);
		NotifyInventoryItemChanged(true, NewItem);
		bChanged = true;
	}

	if (bAutoSlot)
	{
		bChanged |= FillEmptySlotWithItem(NewItem);
	}

	if (bChanged)
	{
		SaveInventory();
		return  true;
	}
	return  false;
}

bool AKayPlayerController::RemoveInventoryItem(UKayDataAsset* RemovedItem, int32 RemoveCount)
{
	if (!RemovedItem)
	{
		UE_LOG(LogKay, Warning, TEXT("RemoveInventoryItem: Failed trying to remove null item!"));
		return false;
	}

	FKayItemData NewData;
	GetInventoryItemData(RemovedItem, NewData);

	if (!NewData.IsValid())
	{
		return false;
	}

	if (RemoveCount <= 0)
	{
		NewData.ItemCount = 0;
	}
	else
	{
		NewData.ItemCount -= RemoveCount;
	}

	if (NewData.ItemCount > 0)
	{
		InventoryData.Add(RemovedItem, NewData);
	}
	else
	{
		InventoryData.Remove(RemovedItem);

		for (TPair<FKayItemSlot, UKayDataAsset*>& Pair : SlottedItems)
		{
			if (Pair.Value == RemovedItem)
			{
				Pair.Value = nullptr;
				NotifySlottedItemChanged(Pair.Key, Pair.Value);
			}
		}
	}

	NotifyInventoryItemChanged(false, RemovedItem);
	SaveInventory();
	return true;
}

void AKayPlayerController::GetInventoryItems(TArray<UKayDataAsset*>& Items, FPrimaryAssetType ItemType)
{
	for (const TPair<UKayDataAsset*, FKayItemData>& Pair : InventoryData)
	{
		if (Pair.Key)
		{
			FPrimaryAssetId AssetId = Pair.Key->GetPrimaryAssetId();

			if (AssetId.PrimaryAssetType == ItemType || !ItemType.IsValid())
			{
				Items.Add(Pair.Key);
			}
		}
	}
}

int32 AKayPlayerController::GetInventoryItemCount(UKayDataAsset* Item) const
{
	const FKayItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		return FoundItem->ItemCount;
	}
	return 0;
}

bool AKayPlayerController::GetInventoryItemData(UKayDataAsset* Item, FKayItemData& ItemData) const
{
	const FKayItemData* FoundItem = InventoryData.Find(Item);

	if (FoundItem)
	{
		ItemData = *FoundItem;
		return true;
	}
	ItemData = FKayItemData(0,0);
	return false;	
}

bool AKayPlayerController::SetSlottedItem(FKayItemSlot ItemSlot, UKayDataAsset* Item)
{
	bool bFound = false;
	for (TPair<FKayItemSlot, UKayDataAsset*>& Pair : SlottedItems)
	{
		if (Pair.Key == ItemSlot)
		{
			bFound = true;
			Pair.Value = Item;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
		else if (Item != nullptr && Pair.Value == Item)
		{
			Pair.Value = nullptr;
			NotifySlottedItemChanged(Pair.Key, Pair.Value);
		}
	}

	if (bFound)
	{
		SaveInventory();
		return true;
	}
	return false;
}

UKayDataAsset* AKayPlayerController::GetSlottedItem(FKayItemSlot ItemSlot) const
{
	auto FoundItem = SlottedItems.Find(ItemSlot);
	if (FoundItem)
	{
		return *FoundItem;
	}

	return nullptr;
}

void AKayPlayerController::GetSlottedItems(TArray<UKayDataAsset*>& Items, FPrimaryAssetType ItemType,
	bool bOutputEmptyIndexes)
{
	for (TPair<FKayItemSlot, UKayDataAsset*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == ItemType || !ItemType.IsValid())
		{
			Items.Add(Pair.Value);
		}
	}
}

void AKayPlayerController::FillEmptySlots()
{
	bool bShouldSave = false;
	for	(const TPair<UKayDataAsset*, FKayItemData>& Pair : InventoryData)
	{
		bShouldSave |= FillEmptySlotWithItem(Pair.Key);
	}

	if (bShouldSave)
	{
		SaveInventory();
	}
}

bool AKayPlayerController::SaveInventory()
{
	auto World = GetWorld();
	UKayGameInstanceSubsystem* GameInstance = World ? World->GetGameInstance()->GetSubsystem<UKayGameInstanceSubsystem>() : nullptr;

	if (!GameInstance)
	{
		return false;
	}

	UKaySaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	if (CurrentSaveGame)
	{
		CurrentSaveGame->InventoryData.Reset();
		CurrentSaveGame->SlottedItems.Reset();

		for (const TPair<UKayDataAsset*, FKayItemData>& ItemPair : InventoryData)
		{
			FPrimaryAssetId AssetId;

			if (ItemPair.Key)
			{
				AssetId = ItemPair.Key->GetPrimaryAssetId();
				CurrentSaveGame->InventoryData.Add(AssetId, ItemPair.Value);
			}
		}

		for (const TPair<FKayItemSlot, UKayDataAsset*>& SlotPair : SlottedItems)
		{
			FPrimaryAssetId AssetId;

			if (SlotPair.Value)
			{
				AssetId = SlotPair.Value->GetPrimaryAssetId();
			}
			CurrentSaveGame->SlottedItems.Add(SlotPair.Key, AssetId);
		}

		GameInstance->WriteSaveGame();
		return true;
	}
	return false;
}

bool AKayPlayerController::LoadInventory()
{
	InventoryData.Reset();
	SlottedItems.Reset();

	UWorld* World = GetWorld();
	UKayGameInstanceSubsystem* GameInstance = World ? World->GetGameInstance()->GetSubsystem<UKayGameInstanceSubsystem>() : nullptr;
	if (!GameInstance)
	{
		return false;
	}

	if (!GameInstance->OnSaveGameLoadedNative.IsBoundToObject(this))
	{
		GameInstance->OnSaveGameLoadedNative.AddUObject(this, &AKayPlayerController::HandleSaveGameLoaded);
	}

	for (const TPair<FPrimaryAssetType, int32>& Pair : GameInstance->ItemSlotsPerType)
	{
		for (int32 SlotNumber = 0; SlotNumber < Pair.Value; ++SlotNumber)
		{
			SlottedItems.Add(FKayItemSlot(Pair.Key,SlotNumber),nullptr);
		}
	}

	UKaySaveGame* CurrentSaveGame = GameInstance->GetCurrentSaveGame();
	UKayAssetManager& AssetManager = UKayAssetManager::Get();
	if (CurrentSaveGame)
	{
		bool bFoundAnySlots = false;
		for (const TPair<FPrimaryAssetId, FKayItemData>& ItemPair : CurrentSaveGame->InventoryData)
		{
			UKayDataAsset* LoadedItem = AssetManager.ForceLoadItem(ItemPair.Key);

			if (LoadedItem != nullptr)
			{
				InventoryData.Add(LoadedItem, ItemPair.Value);
			}
		}

		for (const TPair<FKayItemSlot, FPrimaryAssetId>& SlotPair : CurrentSaveGame->SlottedItems)
		{
			if (SlotPair.Value.IsValid())
			{
				UKayDataAsset* LoadedItem = AssetManager.ForceLoadItem(SlotPair.Value);
				if (GameInstance->IsValidItemSlot(SlotPair.Key) && LoadedItem)
				{
					SlottedItems.Add(SlotPair.Key, LoadedItem);
					bFoundAnySlots = true;
				}
			}
		}

		if (!bFoundAnySlots)
		{
			FillEmptySlots();
		}

		NotifyInventoryLoaded();
		
		return true;
	}
	
	NotifyInventoryLoaded();
	
	return false;
}

bool AKayPlayerController::FillEmptySlotWithItem(UKayDataAsset* NewItem)
{
	FPrimaryAssetType NewItemType = NewItem->GetPrimaryAssetId().PrimaryAssetType;
	FKayItemSlot EmptySlot;
	for (TPair<FKayItemSlot, UKayDataAsset*>& Pair : SlottedItems)
	{
		if (Pair.Key.ItemType == NewItemType)
		{
			if (Pair.Value == NewItem)
			{
				return false;
			}
			else if (Pair.Value == nullptr && (!EmptySlot.IsValid() || EmptySlot.SlotNumber > Pair.Key.SlotNumber))
			{
				EmptySlot = Pair.Key;
			}
		}
	}

	if (EmptySlot.IsValid())
	{
		SlottedItems[EmptySlot] = NewItem;
		NotifySlottedItemChanged(EmptySlot, NewItem);
		return true;
	}
	return false;
}

void AKayPlayerController::NotifyInventoryItemChanged(bool bAdded, UKayDataAsset* Item)
{
	OnInventoryItemChangedNative.Broadcast(bAdded, Item);
	OnInventoryItemChanged.Broadcast(bAdded,Item);

	InventoryItemChanged(bAdded, Item);
}

void AKayPlayerController::NotifySlottedItemChanged(FKayItemSlot ItemSlot, UKayDataAsset* Item)
{
	OnSlottedItemChangedNative.Broadcast(ItemSlot, Item);
	OnSlottedItemChanged.Broadcast(ItemSlot, Item);

	SlottedItemChanged(ItemSlot, Item);
}

void AKayPlayerController::NotifyInventoryLoaded()
{
	OnInventoryLoadedNative.Broadcast();
	OnInventoryLoaded.Broadcast();
}

void AKayPlayerController::HandleSaveGameLoaded(UKaySaveGame* NewSaveGame)
{
	LoadInventory();
}

