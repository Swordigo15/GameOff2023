// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ThrowerCharacter.generated.h"

class ASAPlayerController;
class APickup;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
struct FInputActionValue;

UCLASS()
class SUPPLYAPOCALYPSE_API AThrowerCharacter : public APawn
{
	GENERATED_BODY()

public:
	AThrowerCharacter();

	// ===== Lifecycles ========== //

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Default Initializer ========== //

	void DefaultAssetsInitializer();

	// ===== Game Frameworks ========== //

	UPROPERTY()
	TWeakObjectPtr<ASAPlayerController> PlayerController;

	FORCEINLINE const bool CheckPlayerController();

	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	// ===== Inputs ========== //

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputAction> ChangeModeAction;

	void Look(const FInputActionValue& InputValue);
	void ChangeMode();

	// ===== Game Objects ========== //

	UPROPERTY(EditInstanceOnly)
	TWeakObjectPtr<APickup> Pickup;

	// ===== Camera ========== //

	float ClampCamera(float Value, float Max);
};
