#pragma once

#include "CoreMinimal.h"
#include "Dungeon/GAS/KayAbilityTypes.h"

#include "AbilityProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class DUNGEON_API AAbilityProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbilityProjectileBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FKayGameplayEffectContainerSpec EffectContainerSpec;
	
	UPROPERTY()
	TArray<AActor*> HitActors;
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	virtual void HitDisplay();
};
