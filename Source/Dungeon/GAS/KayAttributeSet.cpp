// Fill out your copyright notice in the Description page of Project Settings.


#include "KayAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Dungeon/Character/KayCharacter.h"
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

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health,MaxHealth,NewValue,GetHealthAttribute());
	}
	else if (Attribute == GetMaxManaAttribute())
	{
		AdjustAttributeForMaxChange(Mana,MaxMana,NewValue,GetManaAttribute());
	}
}

void UKayAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	float DeltaValue = 0;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type:: Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}

	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AKayCharacter* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AKayCharacter>(TargetActor);
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		AActor* SourceActor = nullptr;
		AController* SourceController = nullptr;
		AKayCharacter* SourceCharacter = nullptr;

		if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
			SourceController = Source->AbilityActorInfo->PlayerController.Get();
			if (SourceController == nullptr && SourceActor != nullptr)
			{
				if (APawn* Pawn = Cast<APawn>(SourceActor))
				{
					SourceController = Pawn->GetController();
				}
			}

			if (SourceController)
			{
				SourceCharacter = Cast<AKayCharacter>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter = Cast<AKayCharacter>(SourceActor);
			}
			if (Context.GetEffectCauser())
			{
				SourceActor = Context.GetEffectCauser();
			}
		}

		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult = *Context.GetHitResult();
		}

		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.f)
		{
			const float OldHealth = GetHealth();
			SetHealth(FMath::Clamp(OldHealth - LocalDamageDone, 0.0f, GetMaxHealth()));

			if (TargetCharacter)
			{
				TargetCharacter->HandleDamage(LocalDamageDone, HitResult, SourceTags, SourceCharacter, SourceActor);

				// 这里可以不用调用了 已经监听的生命的属性变化回调
				//TargetCharacter->HandleHealthChanged(-LocalDamageDone, SourceTags);
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		//
		// if (TargetCharacter)
		// {
		// 	// 这里来触发 生命变化 应该是怕 属性频繁的变化（容错处理的）
		// 	// TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
		// }
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// Clamp mana
		// SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
		//
		// if (TargetCharacter)
		// {
		// 	// Call for all mana changes
		// 	TargetCharacter->HandleManaChanged(DeltaValue, SourceTags);
		// }
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute())
	{
		// if (TargetCharacter)
		// {
		// 	// Call for all movespeed changes
		// 	TargetCharacter->HandleMoveSpeedChanged(DeltaValue, SourceTags);
		// }
	}
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
