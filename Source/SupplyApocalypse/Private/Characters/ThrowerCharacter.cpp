// Copyright Anrility. All Rights Reserved.

#include "Characters/ThrowerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Vehicles/Pickup.h"

AThrowerCharacter::AThrowerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));

	// Skeletal Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}

// ==================== Lifecycles ==================== //

void AThrowerCharacter::OnConstruction(const FTransform& Transform)
{
	// Attaching to the pickup/vehicle
	if (Pickup.IsValid())
	{
		FAttachmentTransformRules AttachPickupRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			true
		);

		AttachToActor(Pickup.Get(), AttachPickupRules, TEXT("Standing"));
	}
}

void AThrowerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AThrowerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookAt(DeltaTime);
}

// ==================== Inputs ==================== //

void AThrowerCharacter::Look(const FVector& CrosshairPosition)
{
	bShouldLook = true;
	LookTarget  = CrosshairPosition;
}

// ==================== Camera ==================== //

void AThrowerCharacter::LookAt(float DeltaTime)
{
	if (!bShouldLook) return;

	// Get Rotation
	FRotator CurrentRot = GetActorRotation();
	FRotator TargetRot  = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookTarget);

	// Interpolate the rotation to get smooth transition
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, 3.f);
	NewRot.Roll = NewRot.Pitch = 0.f; /** We only care about the yaw */

	// Stop the interpolation once its nearly equal
	if (FMath::IsNearlyEqual(TargetRot.Yaw, NewRot.Yaw, 1.f))
		bShouldLook = false;

	// Apply if not exceed the limit
	ClampView(NewRot);
}

void AThrowerCharacter::ClampView(const FRotator& NewRot)
{
	// Get the value for the rotation
	float Delta = NewRot.Yaw - GetActorRotation().Yaw;

	// Get vectors
	const FVector PickupBack       = Pickup->GetActorForwardVector() * -1.f;
	const FVector CharacterForward = 		 GetActorForwardVector();
	const FVector CharacterRight   = 		 GetActorRightVector() * -1.f;

	// Get the direction using cross product
	bool bLeftSided = FVector::DotProduct(CharacterRight, PickupBack) > 0.f;
	float NegDelta  = Delta > 0.f ? 0.f   : Delta;
	float PosDelta  = Delta > 0.f ? Delta : 0.f;
	float ClampedYaw = bLeftSided ? NegDelta : PosDelta;

	// Calcualate the angle between em using dot products
	float CosX  = FVector::DotProduct(CharacterForward, PickupBack);
	float Angle = FMath  ::Acos(CosX) * 180.f / UE_PI;
	bool bWithinClamped = Angle <= MaximumView;

	Delta = bWithinClamped ? Delta : ClampedYaw;
	
	AddActorWorldRotation(FRotator(0.f, Delta, 0.f));
}
