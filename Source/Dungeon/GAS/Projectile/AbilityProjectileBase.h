#pragma once

#include "CoreMinimal.h"

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
};
