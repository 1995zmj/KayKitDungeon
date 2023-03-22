


#include "AbilityProjectileBase.h"
#include "Components/SphereComponent.h"
#include "Dungeon/Tool/KayBlueprintFunctionLibrary.h"
#include "Dungeon/Tool/KayLog.h"
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

void AAbilityProjectileBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UE_LOG(LogKay, Log, TEXT("NotifyActorBeginOverlap %s"), *OtherActor->GetName());

	if (!HitActors.Contains(OtherActor))
	{
		HitActors.Add(OtherActor);
		if (GetInstigator() != OtherActor)
		{
			TArray<AActor*> Temp;
			TArray<FHitResult> TempHitResults;
			Temp.Add(OtherActor);
			auto ContainerSpec = UKayBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(EffectContainerSpec,TempHitResults, Temp);
			UKayBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(ContainerSpec);
		}
	}
}

void AAbilityProjectileBase::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	
	HitDisplay();
}

void AAbilityProjectileBase::HitDisplay()
{
	UE_LOG(LogKay, Log, TEXT("Hit Display"));
	Destroy();
}

