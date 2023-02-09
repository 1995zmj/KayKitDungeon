// Fill out your copyright notice in the Description page of Project Settings.


#include "KayAttributeSet.h"

#include "Net/UnrealNetwork.h"

UKayAttributeSet::UKayAttributeSet()
	: Health(1.0f)
	, MaxHealth(1.0f)
{
}

void UKayAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKayAttributeSet, Health);
	DOREPLIFETIME(UKayAttributeSet, MaxHealth);
}

void UKayAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, Health, OldValue);
}

void UKayAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, MaxHealth, OldValue);
}
