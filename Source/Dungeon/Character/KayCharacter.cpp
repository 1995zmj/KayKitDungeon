// Fill out your copyright notice in the Description page of Project Settings.


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

