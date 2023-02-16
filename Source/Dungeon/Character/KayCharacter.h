// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"

#include "KayCharacter.generated.h"

class UKayAbilitySystemComponent;
class UGameplayAbility;
class UKayAttributeSet;
class AKayWeapon;

UCLASS()
class DUNGEON_API AKayCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKayCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Implement IAbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void GiveAbility(TSubclassOf<UGameplayAbility> Ability);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="CharacterBase")
	UKayAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CharacterBase")
	AKayWeapon* CurrentWeapon;

protected:
	UPROPERTY()
	UKayAttributeSet* AttributeSet;
private:
	// UPROPERTY()
	// UKayAbilitySystemComponent* AbilitySystemComponent;
};
