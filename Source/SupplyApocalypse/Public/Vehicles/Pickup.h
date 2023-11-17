// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pickup.generated.h"

class UBoxComponent;
class UChaosWheeledVehicleMovementComponent;
class UCameraComponent;
class UInputAction;
class USpringArmComponent;

struct FInputActionValue;

/** Pickup here means vehicle pickup type */
UCLASS()
class SUPPLYAPOCALYPSE_API APickup : public APawn
{
	GENERATED_BODY()
	
public:	
	APickup();

	// ===== Lifecycles ========== //

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Initializer ========== //

	void DefaultInitializer();

	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> PickupMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UChaosWheeledVehicleMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	// ===== Inputs ========== //

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> HandbrakeAction;

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> AimAction;

	void Move(const FInputActionValue& InputValue);
	void Handbrake(const FInputActionValue& InputValue);
};
