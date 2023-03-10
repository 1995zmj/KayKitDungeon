// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "KayGameplayAbility.h"
#include "KayAbilitySystemComponent.generated.h"


// UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
UCLASS()
class DUNGEON_API UKayAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKayAbilitySystemComponent();

	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UKayGameplayAbility*>& ActiveAbilities);

	static UKayAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);
};
