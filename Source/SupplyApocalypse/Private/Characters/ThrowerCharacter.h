// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowerCharacter.generated.h"

class APickup;

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

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	// ===== Game Objects ========== //

	UPROPERTY(EditInstanceOnly, Category=GameObjects)
	TWeakObjectPtr<APickup> Pickup;

	// ===== View ========== //

	UPROPERTY(EditAnywhere, Category=View, meta=(Units="Degrees"))
	float MaximumView = 75.f;

	bool    bShouldLook;
	FVector LookTarget;

	void LookAt(float DeltaTime);
	void ClampView(const FRotator& NewRot);
};
