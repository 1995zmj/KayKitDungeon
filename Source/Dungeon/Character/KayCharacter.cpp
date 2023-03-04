﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KayCharacter.h"

#include "Dungeon/GAS/KayAbilitySystemComponent.h"
#include "Dungeon/GAS/KayAttributeSet.h"


// Sets default values
AKayCharacter::AKayCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UKayAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UKayAttributeSet>(TEXT("AttributeSet"));
	
	CharacterLevel = 1;
	bAbilitiesInitialized = false;
}

void AKayCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	UE_LOG(LogTemp,Warning,TEXT("zmj InitAbilityActorInfo"));

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKayAttributeSet::GetHealthAttribute()).AddUObject(this, &AKayCharacter::HandleHealthChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKayAttributeSet::GetManaAttribute()).AddUObject(this, &AKayCharacter::HandleManaChanged);
		AddStartupGameplayAbilities();
	}
}

void AKayCharacter::UnPossessed()
{
	Super::UnPossessed();
}

// Called when the game starts or when spawned
void AKayCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKayCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKayCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AKayCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AKayCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AKayCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AKayCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

UAbilitySystemComponent* AKayCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AKayCharacter::GiveAbility(TSubclassOf<UGameplayAbility> Ability)
{
	if (AbilitySystemComponent)
	{
		if (Ability)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1));
		}
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
	}
}

const AKayWeapon* AKayCharacter::GetCurrentWeapon()
{
	return CurrentWeapon;
}

int32 AKayCharacter::GetCharacterLevel() const
{
	return CharacterLevel;
}

float AKayCharacter::GetHealth()
{
	return AttributeSet->GetHealth();
}

float AKayCharacter::GetMaxHealth()
{
	return AttributeSet->GetMaxHealth();
}

float AKayCharacter::GetMana()
{
	return AttributeSet->GetMana();
}

float AKayCharacter::GetMaxMana()
{
	return AttributeSet->GetMaxMana();
}

float AKayCharacter::GetAttributeData(const FGameplayAttribute& Attribute)
{
	return Attribute.GetGameplayAttributeData(AttributeSet)->GetCurrentValue();
}

void AKayCharacter::AddStartupGameplayAbilities()
{

	check(AbilitySystemComponent);

	if (!bAbilitiesInitialized)
	{

		for (TSubclassOf<UKayGameplayAbility>& StartupAbility : GameplayAbilities)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
		
		for (TSubclassOf<UGameplayEffect>& GameplayEffect : PassiveGameplayEffects)
		{
			auto EffectContext = AbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
			if (NewHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
			}
		}
		bAbilitiesInitialized = true;
	}
}

void AKayCharacter::RemoveStartupGameplayAbilities()
{
	if (bAbilitiesInitialized)
	{
		TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
		for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
		{
			if ((Spec.SourceObject == this) && GameplayAbilities.Contains(Spec.Ability->GetClass()))
			{
				AbilitiesToRemove.Add(Spec.Handle);
			}
		}

		for (int32 i = 0; i < AbilitiesToRemove.Num(); ++i)
		{
			AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
		}

		FGameplayEffectQuery Query;
		Query.EffectSource = this;
		AbilitySystemComponent->RemoveActiveEffects(Query);

		bAbilitiesInitialized = false;
	}
}

void AKayCharacter::HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnHealthChanged(DeltaValue, EventTags);
	}
}

void AKayCharacter::HandleHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	if (bAbilitiesInitialized)
	{
		FGameplayTagContainer EventTags;
		OnHealthChanged(AttributeChangeData.NewValue, EventTags);
	}
}

void AKayCharacter::HandleManaChanged(float DeltaValue, const FGameplayTagContainer& EventTags)
{
	if (bAbilitiesInitialized)
	{
		OnManaChanged(DeltaValue, EventTags);
	}
}

void AKayCharacter::HandleManaChanged(const FOnAttributeChangeData& AttributeChangeData)
{
	if (bAbilitiesInitialized)
	{
		FGameplayTagContainer EventTags;
		OnManaChanged(AttributeChangeData.NewValue, EventTags);
	}
}

