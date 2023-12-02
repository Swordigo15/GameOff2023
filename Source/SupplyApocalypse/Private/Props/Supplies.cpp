// Copyright Anrility. All Rights Reserved.

#include "Props/Supplies.h"
#include "Vehicles/Pickup.h"

#if WITH_EDITOR
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#endif

ASupplies::ASupplies()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));
}

// ==================== Lifecycles ==================== //

void ASupplies::OnConstruction(const FTransform& Transform)
{
	// Attach to the truck/pickup
	if (Pickup.IsValid())
	{
		FAttachmentTransformRules AttachPickupRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);

		AttachToActor(Pickup.Get(), AttachPickupRules, TEXT("Supplies"));
	}

	// Place Supplies
	PlaceSupplies();
}

void ASupplies::BeginPlay()
{
	Super::BeginPlay();

}

// ==================== Attributes ==================== //

void ASupplies::PlaceSupplies()
{
	bool bShouldPlace = SupplyMeshes.IsEmpty() || SupplyMeshes.Num() != MaxCount;

	if (MeshesAsset.IsEmpty() || !bShouldPlace) return;

	// Update CurrentCount
	CurrentCount = MaxCount;

	// Remove existing mesh if any from the rear, so removal can be done in O(1) using Pop
	if (!SupplyMeshes.IsEmpty())
		for (int8 I = SupplyMeshes.Num() - 1; I >= 0; --I)
		{
			RemoveInstanceComponent(SupplyMeshes[I]);
			RemoveOwnedComponent(SupplyMeshes[I]);
			SupplyMeshes[I]->DestroyComponent();

			SupplyMeshes.Pop();
		}

	for (uint8 I = 0; I < MaxCount; ++I)
	{
		// Create the mesh and making sure its visible to details panel
		FName ComponentName = *FString::Printf(TEXT("Mesh_%d"), I);
		UStaticMeshComponent* Mesh = NewObject<UStaticMeshComponent>(this, ComponentName);

		Mesh->SetupAttachment(RootComponent);
		AddInstanceComponent(Mesh);
		AddOwnedComponent(Mesh);
		Mesh->RegisterComponent();

		// Randomly choose which static mesh
		uint8 Random = FMath::RandRange(0, MeshesAsset.Num() - 1);
		Mesh->SetStaticMesh(MeshesAsset[Random].LoadSynchronous());

		// Disable any collision
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		SupplyMeshes.Push(Mesh);
	}
}

// ==================== EDITOR ONLY ==================== //

#if WITH_EDITOR
void ASupplies::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	GUnrealEd->UpdateFloatingPropertyWindows();
}
#endif