// Fill out your copyright notice in the Description page of Project Settings.


#include "KayWeapon.h"
#include "Dungeon/Dungeon.h"
#include "Components/CapsuleComponent.h"

FName AKayWeapon::RootComponentName(TEXT("Root"));
FName AKayWeapon::MeshComponentName(TEXT("Mesh"));
FName AKayWeapon::CapsuleComponentName(TEXT("CollisionCylinder"));

// Sets default values
AKayWeapon::AKayWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(AKayWeapon::RootComponentName);

	CapsuleComponent = CreateOptionalDefaultSubobject<UCapsuleComponent>(AKayWeapon::CapsuleComponentName);
	if (CapsuleComponent)
	{
		CapsuleComponent->SetupAttachment(RootComponent);
	}
	
	Mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(AKayWeapon::MeshComponentName);
	if (Mesh)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		Mesh->SetupAttachment(RootComponent);
	}

	bIsAtttacking = false;
}

// Called when the game starts or when spawned
void AKayWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwner %s "), *GetOwner()->GetName());
	}
	if (GetOuter())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOuter %s "), *GetOuter()->GetName());
	}

	// CapsuleComponent->IgnoreActorWhenMoving();
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

// Called every frame
void AKayWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DECLARE_SCOPE_CYCLE_COUNTER(TEXT("AKayWeapon::Tick"), STAT_AKayWeaponTick, STATGROUP_KayD);
}

void AKayWeapon::NotifyBeginWeaponAttack()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	bIsAtttacking = true;
}

void AKayWeapon::NotifyEndWeaponAttack()
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	bIsAtttacking = false;
}

void AKayWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (bIsAtttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("attack %s "), *OtherActor->GetName());
	}
}

void AKayWeapon::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	
}

