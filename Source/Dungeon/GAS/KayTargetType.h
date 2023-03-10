// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "UObject/Object.h"
#include "KayTargetType.generated.h"

class AKayCharacter;

UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class DUNGEON_API UKayTargetType : public UObject
{
	GENERATED_BODY()

public:
	UKayTargetType(){}

	UFUNCTION(BlueprintNativeEvent)
	void GetTargets(AKayCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData,
		TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;

	virtual void GetTargets_Implementation(AKayCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData,
		TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

UCLASS()
class UKayTargetType_UseOwner : public UKayTargetType
{
	GENERATED_BODY()
public:
	UKayTargetType_UseOwner(){}

	virtual void GetTargets_Implementation(AKayCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData,
		TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

UCLASS(NotBlueprintable)
class UKayTargetType_UseEventData : public UKayTargetType
{
	GENERATED_BODY()
public:
	UKayTargetType_UseEventData(){}

	virtual void GetTargets_Implementation(AKayCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData,
		TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};



