// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "KayAbilitySystemComponent.h"
#include "KayAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class DUNGEON_API UKayAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UKayAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_AttackPower)
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UKayAttributeSet, Damage)
	
protected:
	
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float  NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
	
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_AttackPower(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_DefensePower(const FGameplayAttributeData& OldValue);
	
	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldValue);
};
