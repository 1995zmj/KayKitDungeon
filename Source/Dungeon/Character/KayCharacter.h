// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Dungeon/GAS/KayGameplayAbility.h"

#include "KayCharacter.generated.h"

class UKayAbilitySystemComponent;
class UGameplayAbility;
class UKayAttributeSet;
class AKayWeapon;
class UGameplayEffect;

UCLASS()
class DUNGEON_API AKayCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKayCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void UnPossessed() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

public:

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CharacterBase")
	UKayAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
	AKayWeapon* CurrentWeapon;

	UFUNCTION(BlueprintCallable)
	const AKayWeapon* GetCurrentWeapon();

	UFUNCTION(BlueprintCallable)
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth();
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();
	UFUNCTION(BlueprintCallable)
	float GetMana();
	UFUNCTION(BlueprintCallable)
	float GetMaxMana();

	UFUNCTION(BlueprintCallable)
	float GetAttributeData(const FGameplayAttribute& Attribute);
	
protected:
	UPROPERTY()
	UKayAttributeSet* AttributeSet;

	UPROPERTY()
	int32 bAbilitiesInitialized;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 CharacterLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<UKayGameplayAbility>> GameplayAbilities;
	
	void AddStartupGameplayAbilities();

	void RemoveStartupGameplayAbilities();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaBalue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaBalue, const struct FGameplayTagContainer& EventTags);

	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(const FOnAttributeChangeData& AttributeChangeData);

private:
	// UPROPERTY()
	// UKayAbilitySystemComponent* AbilitySystemComponent;
};
