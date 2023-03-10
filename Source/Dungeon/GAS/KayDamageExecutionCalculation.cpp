// Fill out your copyright notice in the Description page of Project Settings.


#include "KayDamageExecutionCalculation.h"

#include "KayAttributeSet.h"

struct KayDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	KayDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKayAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKayAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UKayAttributeSet, Damage, Source, true);
	}
};

static const KayDamageStatics& DamageStatics()
{
	static KayDamageStatics DmgStatics;
	return DmgStatics;
}

UKayDamageExecutionCalculation::UKayDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UKayDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	auto SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	auto SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	auto TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags =  Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags =  Spec.CapturedTargetTags.GetAggregatedTags();

	float DefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefensePowerDef, EvaluationParameters, DefensePower);
	if (DefensePower == 0.0f)
	{
		DefensePower = 1.f;
	}
	
	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackPowerDef, EvaluationParameters, AttackPower);
	
	float Damage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);


	float DamageDone = Damage * AttackPower / DefensePower;

	if (DamageDone)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	}
}
