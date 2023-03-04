// Fill out your copyright notice in the Description page of Project Settings.


#include "KayAttributeSet.h"

#include "Net/UnrealNetwork.h"

UKayAttributeSet::UKayAttributeSet()
	: Health(1.0f)
	, MaxHealth(1.0f)
	, Mana(0.f)
	, MaxMana(0.f)
	, AttackPower(1.0f)
	, DefensePower(1.0f)
	, MoveSpeed(1.0f)
	, Damage(0.0f)
{
}

void UKayAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// if (Attribute == GetMaxHealthAttribute())
	// {
	// 	AdjustAttributeForMaxChange(Health,MaxHealth,NewValue,GetHealthAttribute());
	// }
	// else if (Attribute == GetMaxManaAttribute())
	// {
	// 	AdjustAttributeForMaxChange(Mana,MaxMana,NewValue,GetManaAttribute());
	// }
}

void UKayAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UKayAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKayAttributeSet, Health);
	DOREPLIFETIME(UKayAttributeSet, MaxHealth);
	DOREPLIFETIME(UKayAttributeSet, Mana);
	DOREPLIFETIME(UKayAttributeSet, MaxMana);
	DOREPLIFETIME(UKayAttributeSet, AttackPower);
	DOREPLIFETIME(UKayAttributeSet, DefensePower);
	DOREPLIFETIME(UKayAttributeSet, MoveSpeed);
}

void UKayAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	auto AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();

	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UKayAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, Health, OldValue);
}

void UKayAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, MaxHealth, OldValue);
}

void UKayAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, Mana, OldValue);
}

void UKayAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, MaxMana, OldValue);
}

void UKayAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, AttackPower, OldValue);
}

void UKayAttributeSet::OnRep_DefensePower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, DefensePower, OldValue);
}

void UKayAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKayAttributeSet, MoveSpeed, OldValue);
}
