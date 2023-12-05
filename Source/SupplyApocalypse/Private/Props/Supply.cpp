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
	Collider->SetNotifyRigidBodyCollision(true);

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
	Collider->OnComponentHit.AddDynamic(this, &ThisClass::OnColliderBeginOverlap);

	// ...
	CreateSupply();
}

// ==================== Event Delegates ==================== //

void ASupply::OnColliderBeginOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetWorldTimerManager().IsTimerActive(DestroyTimerHandle)) return;

	// Don't just immediately destroy, give delay
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ThisClass::CreateSupply, DestroyTimer);
}

// ==================== Supply ==================== //

void ASupply::CreateSupply()
{
	if (SupplyMesh.IsEmpty() || !Thrower.IsValid()) return;

	SetOwner(Thrower.Get());

	// Disable any physics
	Collider->SetSimulatePhysics(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Randomly picks what supply to throw
	uint8 Rand = FMath::RandRange(0, SupplyMesh.Num() - 1);
	Mesh->SetStaticMesh(SupplyMesh[Rand].LoadSynchronous());

	// Attach to the thrower character
	USkeletalMeshComponent* ThrowerMesh = Thrower.Get()->GetMesh();
	AttachToComponent(ThrowerMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ThrowSocket"));
}

void ASupply::Throw(float Power)
{
	// This means the supply is thrown
	if (!GetOwner()) return;

	SetOwner(nullptr);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Enable physics
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collider->SetSimulatePhysics(true);

	// Throw away
	FVector Velocity = Thrower->GetThrowUnit() * Power;

	Collider->SetPhysicsLinearVelocity(Velocity);
}
