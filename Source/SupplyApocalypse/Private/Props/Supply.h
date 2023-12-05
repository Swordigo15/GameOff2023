// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Supply.generated.h"

class AThrowerCharacter;
class USphereComponent;

/** The actual supply that will be thrown */
UCLASS()
class SUPPLYAPOCALYPSE_API ASupply : public AActor
{
	GENERATED_BODY()
	
public:	
	ASupply();

	// ===== Supply ========== //

	void CreateSupply();
	FORCEINLINE void Throw(float Power);

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collider;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<UStaticMesh>> SupplyMesh;

	// ===== Game Objects ========== //

	UPROPERTY(EditInstanceOnly, Category=GameObjects)
	TWeakObjectPtr<AThrowerCharacter> Thrower;

	// ===== Event Delegates ========== //

	UFUNCTION()
	void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
