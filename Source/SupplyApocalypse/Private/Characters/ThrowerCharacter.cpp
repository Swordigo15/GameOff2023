// Copyright Anrility. All Rights Reserved.

#include "Characters/ThrowerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Props/Supply.h"
#include "Vehicles/Pickup.h"

AThrowerCharacter::AThrowerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Default Root"));

	// Skeletal Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	// Throw Point
	ThrowPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Throwing Point"));
	ThrowPoint->SetupAttachment(RootComponent);
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

void AThrowerCharacter::AdjustSpeed()
{
	ThrowPower = FMath::Clamp(ThrowPower + PowerDelta, 200.f, 1000.f);

	// Once the power is at clamp, reverse the power delta so it will do the opposite
	// Increasing -> Decreasing, and vice versa
	if (ThrowPower == 200.f || ThrowPower == 1000.f)
		PowerDelta *= -1.f;
}

void AThrowerCharacter::Throw()
{
	Supply->Throw(ThrowPower);

	// Reset Throw Power
	ThrowPower = 100.f;
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

	// Fixing pitch and roll rotation if needed
	FRotator CurrentRot = GetActorRotation();
	if (CurrentRot.Pitch != 0.f || CurrentRot.Roll != 0.f)
		SetActorRotation(FRotator(0.f, CurrentRot.Yaw, 0.f));
}

// ==================== Accessors ==================== //

FVector AThrowerCharacter::GetThrowUnit() const
{
	return (ThrowPoint->GetComponentLocation() - SkeletalMesh->GetSocketLocation(TEXT("ThrowSocket"))).GetSafeNormal();
}
