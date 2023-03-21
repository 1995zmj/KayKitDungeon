// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"
#include "Dungeon/Common/KayInventoryInterface.h"


#include "Dungeon/Common/KayTypes.h"
#include "Dungeon/GAS/KayGameplayAbility.h"
#include "Tests/AutomationTestSettings.h"

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
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


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
	float GetHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable)
	float GetMana() const;
	UFUNCTION(BlueprintCallable)
	float GetMaxMana() const;
	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintCallable)
	bool SetCharacterLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithItemSlot(FKayItemSlot ItemSlot, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithItemSlot(FKayItemSlot ItemSlot, TArray<UKayGameplayAbility*>& ActiveAbilities);
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UKayGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	TMap<FKayItemSlot, TSubclassOf<UKayGameplayAbility>> DefaultSlottedAbilities;

	UPROPERTY()
	TScriptInterface<IKayInventoryInterface> InventorySource;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<FKayItemSlot, FGameplayAbilitySpecHandle> SlottedAbilities;

	FDelegateHandle InventoryUpdateHandle;
	FDelegateHandle InventoryLoadedHandle;
	
	
	void AddStartupGameplayAbilities();

	void RemoveStartupGameplayAbilities();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDamaged(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AKayCharacter* InstigatorCharacter, AActor* DamageCauser);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float DeltaBalue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float DeltaBalue, const struct FGameplayTagContainer& EventTags);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);

	void OnItemSlotChanged(FKayItemSlot ItemSlot, UKayDataAsset* Item);

	void RefreshSlottedGameplayAbilities();


	void AddSlottedGameplayAbilities();

	void FillSlottedAbilitySpecs(TMap<FKayItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs);

	void RemoveSlottedGameplayAbilities(bool bRemoveAll);

	virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AKayCharacter* InstigatorCharacter, AActor* DamageCauser);
	virtual void HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleHealthChanged(const FOnAttributeChangeData& AttributeChangeData);
	virtual void HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags);
	virtual void HandleManaChanged(const FOnAttributeChangeData& AttributeChangeData);

	// virtual FGenericTeamId GetGenericTeamId() const override;

	friend UKayAttributeSet;

private:
	// UPROPERTY()
	// UKayAbilitySystemComponent* AbilitySystemComponent;
};
