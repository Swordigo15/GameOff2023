// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowerCharacter.generated.h"

class APickup;
class ASupply;

UCLASS()
class SUPPLYAPOCALYPSE_API AThrowerCharacter : public AActor
{
	GENERATED_BODY()

public:
	AThrowerCharacter();

	// ===== Lifecycles ========== //

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;

	// ===== Input Handlers ========== //

	FORCEINLINE void Look(const FVector& CrosshairPosition);
	FORCEINLINE void AdjustSpeed();
	FORCEINLINE void Throw();

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> ThrowPoint;

	// ===== Game Objects ========== //

	UPROPERTY(EditInstanceOnly, Category=GameObjects)
	TWeakObjectPtr<APickup> Pickup;

	UPROPERTY(EditInstanceOnly, Category=GameObjects)
	TWeakObjectPtr<ASupply> Supply;

	// ===== Attributes ========== //

	UPROPERTY(EditAnywhere, Category=Attributes)
	float ThrowPower = 100.f;

	/** Will the power increased or decreased */
	float PowerDelta = 30.f;

	// ===== View ========== //

	UPROPERTY(EditAnywhere, Category=View, meta=(Units="Degrees"))
	float MaximumView = 75.f;

	bool    bShouldLook;
	FVector LookTarget;

	void LookAt(float DeltaTime);
	void ClampView(const FRotator& NewRot);

public:
	// ===== Accessors ========== //

	FORCEINLINE USkeletalMeshComponent* GetMesh() const
	{
		return SkeletalMesh;
	}
	FORCEINLINE FVector GetThrowUnit() const;
};
