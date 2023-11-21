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

enum class EVehicleMode : uint8
{
	EVM_Drive = 0,
	EVM_Throw,
	EVM_Both
};

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

	/** For driving mode */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> DriveSpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> DriveCamera;

	/** For throwing mode */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> ThrowSpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ThrowCamera;

	// ===== Inputs ========== //

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> HandbrakeAction;

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> ChangeModeAction;

	void Move(const FInputActionValue& InputValue);
	void Handbrake(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void ChangeMode(const FInputActionValue& InputValue);

	// ===== Attributes ========== //

	EVehicleMode VehicleMode = EVehicleMode::EVM_Drive;

	// ===== Camera ========== //

	/** When the timer is over, will automatically make the camera follow activated */
	float CameraFollowTimer = 5.f;
	FTimerHandle CameraFollowTimerHandle;

	/** Maximum/minimum angle that can be look at when on Driving Mode */
	float DriveAngleLimit = 35.f;

	/** Maximum/minimum angle that can be look at when on Throwing Mode */
	float ThrowAngleLimit = 35.f;

	/** Get the angle between camera and the vehicle's forward/backward/up */
	float GetCameraAngle(const FVector& CameraVector, const FVector& VehicleVector);

	/** Make the camera always follow the vehicle's front/back */
	void CameraFollow(float DeltaTime);
};
