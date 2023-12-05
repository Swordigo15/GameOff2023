// Copyright Anrility. All Rights Reserved.

#include "Props/Supply.h"
#include "Characters/ThrowerCharacter.h"
#include "Components/SphereComponent.h"

ASupply::ASupply()
{
	PrimaryActorTick.bCanEverTick = false;

	// Collider
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	/** Collision **/
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	
	// Binding Delegates
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnColliderBeginOverlap);

	// ...
	SetOwner(Thrower.Get());
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

	// Disable any physics
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetSimulatePhysics(false);

	// Randomly picks what supply to throw
	uint8 Rand = FMath::RandRange(0, SupplyMesh.Num() - 1);
	Mesh->SetStaticMesh(SupplyMesh[Rand].LoadSynchronous());

	// Attach to the thrower character
	USkeletalMeshComponent* ThrowerMesh = Thrower.Get()->GetMesh();
	AttachToComponent(ThrowerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ThrowSocket"));
}

void ASupply::Throw(float Power)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Enable physics
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collider->SetSimulatePhysics(true);

	// Throw away
	FVector Velocity = Thrower->GetThrowUnit() * Power;

	Collider->SetPhysicsLinearVelocity(Velocity);
}
