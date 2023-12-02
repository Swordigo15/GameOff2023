// Copyright Anrility. All Rights Reserved.

#include "Characters/ThrowerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SAPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Vehicles/Pickup.h"

AThrowerCharacter::AThrowerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default value
	bUseControllerRotationPitch = bUseControllerRotationRoll = bUseControllerRotationYaw = false;

	// Root Capsule
	CapsuleRoot = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Root"));

	SetRootComponent(CapsuleRoot);

	// Skeletal Mesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Initialize Assets
	DefaultAssetsInitializer();
}

// ==================== Initializer ==================== //

void AThrowerCharacter::DefaultAssetsInitializer()
{
	// Inputs Initializer
	ConstructorHelpers::FObjectFinder<UInputAction> LookObject(
		TEXT("/Script/EnhancedInput.InputAction'/Game/GameContent/Blueprints/Inputs/IA_Look.IA_Look'")
	);

	LookAction = LookObject.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> ChangeModeObject(
		TEXT("/Script/EnhancedInput.InputAction'/Game/GameContent/Blueprints/Inputs/IA_ChangeMode.IA_ChangeMode'")
	);

	ChangeModeAction = ChangeModeObject.Object;
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

void AThrowerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInput->BindAction(ChangeModeAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::ChangeMode);
	}
}

void AThrowerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ==================== GameFrameworks ==================== //

const bool AThrowerCharacter::CheckPlayerController()
{
	if (!PlayerController.IsValid()) PlayerController = Cast<ASAPlayerController>(GetController());

	return PlayerController.IsValid();
}

// ==================== Inputs ==================== //

void AThrowerCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	AddControllerPitchInput(Value.Y);

	// Handling Yaw
	float ClampYaw = ClampCamera(Value.X, 120.f); 
	AddControllerYawInput(ClampYaw);
}

void AThrowerCharacter::ChangeMode()
{
	if (CheckPlayerController()) PlayerController->Possess(Pickup.Get());
}

// ==================== Camera ==================== //

float AThrowerCharacter::ClampCamera(float Value, float Max)
{
	// Get the forward vectors
	const FVector CharacterForward = GetActorForwardVector();
	const FVector CharacterRight   = GetActorRightVector();
	const FVector CameraForward    = Camera->GetForwardVector();

	// Yaw towards clamped range
	bool bLeftSided = FVector::DotProduct(CharacterRight, CameraForward) > 0.f;
	float NegInput = Value > 0.f ? 0.f   : Value;
	float PosInput = Value > 0.f ? Value : 0.f;

	float ClampedYaw = bLeftSided ? NegInput : PosInput;
	
	// Calcualate the angle between em using dot products
	float CosX  = FVector::DotProduct(CharacterForward, CameraForward);
	float Angle = FMath::Acos(CosX) * 180.f / UE_PI;

	bool bWithinClampedRange = Angle < Max / 2.f;

	return bWithinClampedRange ? Value : ClampedYaw;
}
