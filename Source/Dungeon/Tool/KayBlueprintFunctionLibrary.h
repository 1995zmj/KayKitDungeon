// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dungeon/Common/KayTypes.h"
#include "Dungeon/GAS/KayAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KayBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UKayBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Loading")
	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime);

	UFUNCTION(BlueprintCallable, Category = "Loading")
	static void StopLoadingScreen();

	UFUNCTION(BlueprintPure, Category = "Loading")
	static bool IsInEditor();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Equal (RPGItemSlot)", CompactNodeTitle = "==", Keywords = "== equal"), Category = "Inventory")
	static bool EqualEqual_KayItemSlot(const FKayItemSlot& A, const FKayItemSlot& B);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "NotEqualEqual (RPGItemSlot)", CompactNodeTitle = "!=", Keywords = "!= not equal"), Category = "Inventory")
	static bool NotEqual_RPGItemSlot(const FKayItemSlot& A, const FKayItemSlot& B);

	UFUNCTION(BlueprintPure,  Category = "Inventory")
	static bool IsValidItemSlot(const FKayItemSlot& ItemSlot);

	UFUNCTION(BlueprintPure, Category = "Ability")
	static bool DoesEffectContainerSpecHaveEffects(const FKayGameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintPure, Category = "Ability")
	static bool DoesEffectContainerSpecHaveTargets(const FKayGameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category = "Ability", meta = (AutoCreateRefTerm = "HitResults,TargetActors"))
	static FKayGameplayEffectContainerSpec AddTargetsToEffectContainerSpec(const FKayGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
	
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FKayGameplayEffectContainerSpec& ContainerSpec);
	
	UFUNCTION(BlueprintPure, Category = "Project")
	static FString GetProjectVersion();

};
