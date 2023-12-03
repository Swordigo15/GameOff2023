// Copyright Anrility. All Rights Reserved.

#include "Props/Supply.h"
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
}

// ==================== Event Delegates ==================== //

void ASupply::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
