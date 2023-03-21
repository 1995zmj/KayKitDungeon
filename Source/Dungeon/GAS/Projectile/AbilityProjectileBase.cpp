


#include "AbilityProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// \GameFramework\ProjectileMovementComponent.h

// Sets default values
AAbilityProjectileBase::AAbilityProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// CameraBoom->SetupAttachment(RootComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
}

// Called when the game starts or when spawned
void AAbilityProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAbilityProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

