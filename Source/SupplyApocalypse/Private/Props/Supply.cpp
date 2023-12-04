// Copyright Anrility. All Rights Reserved.

#include "Props/Supply.h"
#include "Characters/ThrowerCharacter.h"
#include "Components/BoxComponent.h"

ASupply::ASupply()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collider
	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	/** Collision **/
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	SetRootComponent(Collider);

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	/** Collision **/
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// ==================== Lifecycles ==================== //

void ASupply::BeginPlay()
{
	Super::BeginPlay();
	
	// Bindin Delegates
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnColliderBeginOverlap);

	// ...
	CreateSupply();
}

// ==================== Event Delegates ==================== //

void ASupply::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

// ==================== Supply ==================== //

void ASupply::CreateSupply()
{
	if (SupplyMesh.IsEmpty() || !Thrower.IsValid()) return;

	// Randomly picks what supply to throw
	uint8 Rand = FMath::RandRange(0, SupplyMesh.Num() - 1);
	Mesh->SetStaticMesh(SupplyMesh[Rand].LoadSynchronous());

	// Attach to the thrower character
	AttachToActor(Thrower.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ThrowSocket"));
}
