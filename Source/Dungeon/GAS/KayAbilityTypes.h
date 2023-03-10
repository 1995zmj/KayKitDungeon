// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "KayTargetType.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "KayAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FKayGameplayEffectContainer
{
	GENERATED_BODY()
public:
	FKayGameplayEffectContainer(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffectContainer")
	TSubclassOf<UKayTargetType> TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffectContainer")
	TArray<TSubclassOf<UGameplayEffect>> TargetGameplayEffectClasses;
};


USTRUCT(BlueprintType)
struct FKayGameplayEffectContainerSpec
{
	GENERATED_BODY()
public:
	FKayGameplayEffectContainerSpec(){}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameplayEffectContainer")
	FGameplayAbilityTargetDataHandle TargetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayEffectContainer")
	TArray<FGameplayEffectSpecHandle> TargetGameplayEffectSpecs;

	bool HasValidEffects() const;

	bool HasValidTargets() const;

	void AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);
};
