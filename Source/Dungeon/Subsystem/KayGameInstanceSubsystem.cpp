// Fill out your copyright notice in the Description page of Project Settings.


#include "KayGameInstanceSubsystem.h"
#include "Dungeon/LocalData/KaySaveGame.h"
#include "Kismet/GameplayStatics.h"

void UKayGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SaveSlot = TEXT("SaveGame_K");
	SaveUserIndex = 0;
	UE_LOG(LogTemp,Log,TEXT("UKayGameInstanceSubsystem Initialize"));
}

void UKayGameInstanceSubsystem::SetSavingEnabled(bool bEnabled)
{
	bSavingEnabled = bEnabled;
}

bool UKayGameInstanceSubsystem::ReadSaveGame()
{
	SetSavingEnabled(true);
	UGameplayStatics::AsyncLoadGameFromSlot(SaveSlot, SaveUserIndex, FAsyncLoadGameFromSlotDelegate::CreateUObject(this,&UKayGameInstanceSubsystem::HandleAsyncLoad));
	return true;
}

bool UKayGameInstanceSubsystem::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		if (bCurrentlySaving)
		{
			bPendingSaveRequested = true;
			return true;
		}

		bCurrentlySaving = true;
		UGameplayStatics::AsyncSaveGameToSlot(CurrentSaveGame, SaveSlot, SaveUserIndex, FAsyncSaveGameToSlotDelegate::CreateUObject(this, &UKayGameInstanceSubsystem::HandleAsyncSave));
		return true;
	}
	return false;
}

void UKayGameInstanceSubsystem::ResetSaveGame()
{
	HandleAsyncLoad(SaveSlot, SaveUserIndex, nullptr);
}

void UKayGameInstanceSubsystem::HandleAsyncLoad(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	if (SaveGame)
	{
		CurrentSaveGame = Cast<UKaySaveGame>(SaveGame);
		ensure(CurrentSaveGame != nullptr);
	}
	else
	{
		CurrentSaveGame = Cast<UKaySaveGame>(UGameplayStatics::CreateSaveGameObject(UKaySaveGame::StaticClass()));
		WriteSaveGame();
	}
}

void UKayGameInstanceSubsystem::HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	ensure(bCurrentlySaving);
	bCurrentlySaving = false;

	// 处理多次加载的问题
	if (bPendingSaveRequested)
	{
		bPendingSaveRequested = false;
		WriteSaveGame();
	}
}
