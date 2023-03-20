// Fill out your copyright notice in the Description page of Project Settings.


#include "KayBlueprintFunctionLibrary.h"

#include "LoadingScreen.h"

void UKayBlueprintFunctionLibrary::PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime)
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
	LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
}

void UKayBlueprintFunctionLibrary::StopLoadingScreen()
{
	ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
	LoadingScreenModule.StopInGameLoadingScreen();
}

bool UKayBlueprintFunctionLibrary::IsInEditor()
{
	return GIsEditor;
}

bool UKayBlueprintFunctionLibrary::EqualEqual_KayItemSlot(const FKayItemSlot& A, const FKayItemSlot& B)
{
	return A == B;
}

bool UKayBlueprintFunctionLibrary::NotEqual_RPGItemSlot(const FKayItemSlot& A, const FKayItemSlot& B)
{
	return A != B;
}

bool UKayBlueprintFunctionLibrary::IsValidItemSlot(const FKayItemSlot& ItemSlot)
{
	return ItemSlot.IsValid();
}

bool UKayBlueprintFunctionLibrary::DoesEffectContainerSpecHaveEffects(
	const FKayGameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidEffects();
}

bool UKayBlueprintFunctionLibrary::DoesEffectContainerSpecHaveTargets(
	const FKayGameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidTargets();
}

FKayGameplayEffectContainerSpec UKayBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(
	const FKayGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults,
	const TArray<AActor*>& TargetActors)
{
	FKayGameplayEffectContainerSpec NewSpec = ContainerSpec;
	NewSpec.AddTargets(HitResults, TargetActors);
	return NewSpec;
}

TArray<FActiveGameplayEffectHandle> UKayBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(
	const FKayGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		if (SpecHandle.IsValid())
		{
			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
			{
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
			}
		}
	}
	return AllEffects;
}

FString UKayBlueprintFunctionLibrary::GetProjectVersion()
{
	FString ProjectVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		ProjectVersion,
		GGameIni
	);

	return ProjectVersion;
}

