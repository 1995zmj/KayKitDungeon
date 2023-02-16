// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Dungeon/Asset/KayDataAsset.h"
#include "Dungeon/Common/KayTypes.h"
#include "KayPlayerController.generated.h"


UCLASS()
class DUNGEON_API AKayPlayerController : public APlayerController
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
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<URPGItem*, FRPGItemData> InventoryData;*/

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FKayItemSlot, UKayDataAsset*> SlottedItems;
};
