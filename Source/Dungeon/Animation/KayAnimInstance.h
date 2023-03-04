// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KayAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_API UKayAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShouldMove;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Speed;
};
