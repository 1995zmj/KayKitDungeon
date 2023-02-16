// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KayWeapon.generated.h"

class UCapsuleComponent;

UCLASS()
class DUNGEON_API AKayWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AKayWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UFUNCTION(Category = Weapon, BlueprintCallable)
	void NotifyBeginWeaponAttack();

	UFUNCTION(Category = Weapon, BlueprintCallable)
	void NotifyEndWeaponAttack();

public:
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(Category = Weapon, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	static FName RootComponentName;
	static FName MeshComponentName;
	static FName CapsuleComponentName;
	
protected:
	bool bIsAtttacking;
};
