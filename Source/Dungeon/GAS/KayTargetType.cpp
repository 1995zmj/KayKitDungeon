// Fill out your copyright notice in the Description page of Project Settings.


#include "KayTargetType.h"
#include "Dungeon/Character/KayCharacter.h"

void UKayTargetType::GetTargets_Implementation(AKayCharacter* TargetingCharacter, AActor* TargetingActor,
	FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}


void UKayTargetType_UseOwner::GetTargets_Implementation(AKayCharacter* TargetingCharacter, AActor* TargetingActor,
	FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void UKayTargetType_UseEventData::GetTargets_Implementation(AKayCharacter* TargetingCharacter, AActor* TargetingActor,
	FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if(EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}
