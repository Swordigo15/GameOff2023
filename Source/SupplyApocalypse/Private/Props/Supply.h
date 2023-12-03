// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Supply.generated.h"

class UBoxComponent;

/** The actual supply that will be thrown */
UCLASS()
class SUPPLYAPOCALYPSE_API ASupply : public AActor
{
	GENERATED_BODY()
	
public:	
	ASupply();

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Collider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	// ===== Event Delegates ========== //

	UFUNCTION()
	void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
