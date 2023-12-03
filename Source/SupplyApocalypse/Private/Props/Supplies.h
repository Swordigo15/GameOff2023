// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Supplies.generated.h"

class APickup;

UCLASS()
class SUPPLYAPOCALYPSE_API ASupplies : public AActor
{
	GENERATED_BODY()
	
public:	
	ASupplies();

	// ===== Lifecycles ========== //

	virtual void OnConstruction(const FTransform& Transform) override;

	// ===== EDITOR ONLY ========== //
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Components ========== //

	UPROPERTY(VisibleInstanceOnly)
	TArray<TObjectPtr<UStaticMeshComponent>> SupplyMeshes;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UStaticMesh>> MeshesAsset;

	// ===== Game Objects ========== //

	UPROPERTY(EditInstanceOnly, Category=GameObjects)
	TWeakObjectPtr<APickup> Pickup;

	// ===== Attributes ========== //

	UPROPERTY(EditAnywhere, Category=Attributes)
	uint8 MaxCount = 10;

	uint8 CurrentCount;

	void PlaceSupplies();
};
