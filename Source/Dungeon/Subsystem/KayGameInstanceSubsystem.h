// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KayGameInstanceSubsystem.generated.h"

// 主要管理本地数据的 名字画面可能要改
class UKaySaveGame;
class USaveGame;

UCLASS()
class DUNGEON_API UKayGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	FString SaveSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Inventory)
	int32 SaveUserIndex;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(BlueprintCallable, Category = Save)
	void SetSavingEnabled(bool bEnabled);
	
	UFUNCTION(BlueprintCallable, Category = Save)
	bool ReadSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
	bool WriteSaveGame();

	UFUNCTION(BlueprintCallable, Category = Save)
    void ResetSaveGame();
protected:
	UPROPERTY()
	UKaySaveGame* CurrentSaveGame;

	UPROPERTY()
	bool bSavingEnabled;

	UPROPERTY()
	bool bCurrentlySaving;
	
	UPROPERTY()
	bool bPendingSaveRequested;

	virtual void HandleAsyncLoad(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);

	virtual void HandleAsyncSave(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
