// Fill out your copyright notice in the Description page of Project Settings.


#include "KayCharacter.h"

#include "Dungeon/GAS/KayAbilitySystemComponent.h"
#include "Dungeon/GAS/KayAttributeSet.h"
#include "Dungeon/Tool/KayLog.h"

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
}

void AKayCharacter::UnPossessed()
{
	Super::UnPossessed();
}

// Called when the game starts or when spawned
void AKayCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("zmj InitAbilityActorInfo"));
		AbilitySystemComponent->InitAbilityActorInfo(this,this);
		// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKayAttributeSet::GetHealthAttribute()).AddUObject(this, &AKayCharacter::HandleHealthChanged);
		// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UKayAttributeSet::GetManaAttribute()).AddUObject(this, &AKayCharacter::HandleManaChanged);
		AddStartupGameplayAbilities();
	}
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
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, GetCharacterLevel(), INDEX_NONE, this));
		}
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

float AKayCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AKayCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AKayCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float AKayCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float AKayCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

bool AKayCharacter::SetCharacterLevel(int32 NewLevel)
{
	if (CharacterLevel != NewLevel && NewLevel > 0)
	{
		RemoveStartupGameplayAbilities();
		CharacterLevel = NewLevel;
		AddStartupGameplayAbilities();
		return true;
	}
	return false;
}

bool AKayCharacter::ActivateAbilitiesWithItemSlot(FKayItemSlot ItemSlot, bool bAllowRemoteActivation)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

	if (FoundHandle && AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbility(*FoundHandle, bAllowRemoteActivation);
	}

	return false;
}

void AKayCharacter::GetActiveAbilitiesWithItemSlot(FKayItemSlot ItemSlot, TArray<UKayGameplayAbility*>& ActiveAbilities)
{
	FGameplayAbilitySpecHandle* FoundHandle = SlottedAbilities.Find(ItemSlot);

	if (FoundHandle && AbilitySystemComponent)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(*FoundHandle);

		if (FoundSpec)
		{
			TArray<UGameplayAbility*> AbilityInstances = FoundSpec->GetAbilityInstances();

			for (UGameplayAbility* ActiveAbility : AbilityInstances)
			{
				ActiveAbilities.Add(Cast<UKayGameplayAbility>(ActiveAbility));
			}
		}
	}
}

bool AKayCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags,bAllowRemoteActivation);
	}
	return false;
}

void AKayCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags,
	TArray<UKayGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool AKayCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining,
	float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;
		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray<TPair<float, float>> DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);

		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}
			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

float AKayCharacter::GetAttributeData(const FGameplayAttribute& Attribute)
{
	return Attribute.GetGameplayAttributeData(AttributeSet)->GetCurrentValue();
}

bool AKayCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

bool AKayCharacter::DoMeleeAttack()
{
	return false;
}

bool AKayCharacter::DoSkillAttack()
{
	return false;
}

bool AKayCharacter::UseItemPotion()
{
	return false;
}

void AKayCharacter::DelayedDestroy()
{
	UE_LOG(LogKay, Warning, TEXT("只有死亡动画之后能调用"));
	HandlePostDeath();
}

void AKayCharacter::InitWeapon()
{
	if (WeaponDataAsset)
	{
		CurrentWeapon =  Cast<AKayWeapon>(GetWorld()->SpawnActor(WeaponDataAsset->WeaponActor));
		CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), FName("HandRightSocket"));
	}
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

void AKayCharacter::OnItemSlotChanged(FKayItemSlot ItemSlot, UKayDataAsset* Item)
{
	RefreshSlottedGameplayAbilities();
}

void AKayCharacter::RefreshSlottedGameplayAbilities()
{
	if (bAbilitiesInitialized)
	{
		RemoveSlottedGameplayAbilities(false);
		AddSlottedGameplayAbilities();
;	}
}

void AKayCharacter::AddSlottedGameplayAbilities()
{
	TMap<FKayItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;
	FillSlottedAbilitySpecs(SlottedAbilitySpecs);

	for (const TPair<FKayItemSlot,FGameplayAbilitySpec>& SpecPair : SlottedAbilitySpecs)
	{
		FGameplayAbilitySpecHandle& SpecHandle = SlottedAbilities.FindOrAdd(SpecPair.Key);
		if (!SpecHandle.IsValid())
		{
			SpecHandle = AbilitySystemComponent->GiveAbility(SpecPair.Value);
		}
	}
	
}

void AKayCharacter::FillSlottedAbilitySpecs(TMap<FKayItemSlot, FGameplayAbilitySpec>& SlottedAbilitySpecs)
{

	for (const TPair<FKayItemSlot, TSubclassOf<UKayGameplayAbility>> DefaultPair : DefaultSlottedAbilities)
	{
		if (DefaultPair.Value.Get())
		{
			SlottedAbilitySpecs.Add(DefaultPair.Key, FGameplayAbilitySpec(DefaultPair.Value, GetCharacterLevel()));
		}
	}

	if (InventorySource)
	{
		const TMap<FKayItemSlot, UKayDataAsset*>& SlottedItemMap = InventorySource->GetSlottedItemMap();

		for (const TPair<FKayItemSlot, UKayDataAsset*>& ItemPair : SlottedItemMap)
		{
			UKayDataAsset* SlottedItem = ItemPair.Value;

			int32 AbilityLevel = GetCharacterLevel();

			if (SlottedItem && SlottedItem->ItemType.GetName() == FName(TEXT("Weapon")))
			{
				AbilityLevel = SlottedItem->AbilityLevel;
			}

			if (SlottedItem && SlottedItem->GrantedAbility)
			{
				SlottedAbilitySpecs.Add(ItemPair.Key, FGameplayAbilitySpec(SlottedItem->GrantedAbility, AbilityLevel));
			}
		}
	}
}


void AKayCharacter::RemoveSlottedGameplayAbilities(bool bRemoveAll)
{
	TMap<FKayItemSlot, FGameplayAbilitySpec> SlottedAbilitySpecs;

	if (!bRemoveAll)
	{
		FillSlottedAbilitySpecs(SlottedAbilitySpecs);
	}

	for (TPair<FKayItemSlot, FGameplayAbilitySpecHandle>& ExistingPair : SlottedAbilities)
	{
		FGameplayAbilitySpec* FoundSpec = AbilitySystemComponent->FindAbilitySpecFromHandle(ExistingPair.Value);

		bool bShouldRemove = bRemoveAll || !FoundSpec;

		if (!bShouldRemove)
		{
			FGameplayAbilitySpec* DesiredSpec = SlottedAbilitySpecs.Find(ExistingPair.Key);
			if (!DesiredSpec || DesiredSpec->Ability != FoundSpec->Ability || DesiredSpec->SourceObject != FoundSpec->SourceObject)
			{
				bShouldRemove = true;
			}
		}

		if (bShouldRemove)
		{
			if (FoundSpec)
			{
				AbilitySystemComponent->ClearAbility(ExistingPair.Value);
			}

			ExistingPair.Value = FGameplayAbilitySpecHandle();
		}
	}
	
}

void AKayCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
                                 AKayCharacter* InstigatorCharacter, AActor* DamageCauser)
{
	OnDamaged(DamageAmount, HitInfo, DamageTags, InstigatorCharacter, DamageCauser);
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
		OnHealthChanged(AttributeChangeData.OldValue - AttributeChangeData.NewValue, EventTags);
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
		OnManaChanged(AttributeChangeData.OldValue - AttributeChangeData.NewValue, EventTags);
	}
}

void AKayCharacter::HandlePreDeath()
{
	if (bAbilitiesInitialized)
	{
		OnPreDeath();
	}
}

void AKayCharacter::HandlePostDeath()
{
	if (bAbilitiesInitialized)
	{
		OnPostDeath();
	}
}

