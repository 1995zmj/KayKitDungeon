#include "KayAbilitySystemComponent.h"
#include "Dungeon/Character/KayCharacter.h"
#include "KayGameplayAbility.h"

FKayGameplayEffectContainerSpec UKayGameplayAbility::MakeEffectContainerSpecFromContainer(
	const FKayGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FKayGameplayEffectContainerSpec ReturnSpec;

	AActor* OwningActor = GetOwningActorFromActorInfo();

	AKayCharacter* OwningCharacter = Cast<AKayCharacter>(OwningActor);
	auto OwningASC = UKayAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;

			const UKayTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();

			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults,TargetActors);
		}

		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = this->GetAbilityLevel();
		}

		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FKayGameplayEffectContainerSpec UKayGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag,
	const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FKayGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FKayGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UKayGameplayAbility::ApplyEffectContainerSpec(
	const FKayGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UKayGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag,
	const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FKayGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}
