// Copyright Anrility. All Rights Reserved.

#include "Vehicles/Pickup.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	// PickupMesh
	PickupMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->SetSimulatePhysics(true);
	PickupMesh->SetMobility(EComponentMobility::Movable);
	PickupMesh->SetNotifyRigidBodyCollision(true);
	PickupMesh->SetGenerateOverlapEvents(true);
	PickupMesh->SetCanEverAffectNavigation(false);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PickupMesh->SetCollisionObjectType(ECollisionChannel::ECC_Vehicle);
	SetRootComponent(PickupMesh);

	// Movement
	Movement = CreateDefaultSubobject<UChaosWheeledVehicleMovementComponent>(TEXT("Vehicle Movement"));
	Movement->UpdatedComponent = PickupMesh;

	// Drive Spring Arm
	DriveSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Drive Spring Arm"));
	DriveSpringArm->SetupAttachment(RootComponent);
	DriveSpringArm->bInheritPitch = DriveSpringArm->bInheritRoll = false;
	DriveSpringArm->bDoCollisionTest = false;
	DriveSpringArm->bEnableCameraRotationLag = true;
	DriveSpringArm->CameraRotationLagSpeed = 2.f;

	// Drive Camera
	DriveCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Drive Camera"));
	DriveCamera->SetupAttachment(DriveSpringArm);

	// Throw Spring Arm
	ThrowSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Throw Spring Arm"));
	ThrowSpringArm->SetupAttachment(RootComponent);
	ThrowSpringArm->bInheritPitch = ThrowSpringArm->bInheritRoll = false;
	ThrowSpringArm->bDoCollisionTest = false;
	ThrowSpringArm->bEnableCameraRotationLag = true;
	ThrowSpringArm->CameraRotationLagSpeed = 2.f;

	// Throw Camera
	ThrowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Throw Camera"));
	ThrowCamera->SetupAttachment(ThrowSpringArm);
	ThrowCamera->Deactivate(); /* Using drive camera for the first time */

	// Initializer
	DefaultInitializer();
}

// ==================== Initializer ==================== //

void APickup::DefaultInitializer()
{
	// Adding wheels to the movement
	FChaosWheelSetup Wheel1, Wheel2, Wheel3, Wheel4;
	ConstructorHelpers::FClassFinder<UChaosVehicleWheel> FrontClass(
		TEXT("/Game/GameContent/Blueprints/Vehicles/BP_FrontWheel")
	);
	ConstructorHelpers::FClassFinder<UChaosVehicleWheel> RearClass(
		TEXT("/Game/GameContent/Blueprints/Vehicles/BP_RearWheel")
	);

	Wheel1.WheelClass = Wheel2.WheelClass = FrontClass.Class;
	Wheel3.WheelClass = Wheel4.WheelClass = RearClass .Class;
	Wheel1.BoneName = TEXT("FL");
	Wheel2.BoneName = TEXT("FR");
	Wheel3.BoneName = TEXT("RL");
	Wheel4.BoneName = TEXT("RR");

	Movement->WheelSetups.Add(Wheel1);
	Movement->WheelSetups.Add(Wheel2);
	Movement->WheelSetups.Add(Wheel3);
	Movement->WheelSetups.Add(Wheel4);

	// Inputs Initializer
	ConstructorHelpers::FObjectFinder<UInputAction> MoveObject(
		TEXT("/Script/EnhancedInput.InputAction'/Game/GameContent/Blueprints/Inputs/IA_Move.IA_Move'")
	);

	MoveAction = MoveObject.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> HandbreakObject(
		TEXT("/Script/EnhancedInput.InputAction'/Game/GameContent/Blueprints/Inputs/IA_Handbrake.IA_Handbrake'")
	);

	HandbrakeAction = HandbreakObject.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> ChangeModeObject(
		TEXT("/Script/EnhancedInput.InputAction'/Game/GameContent/Blueprints/Inputs/IA_ChangeMode.IA_ChangeMode'")
	);

	ChangeModeAction = ChangeModeObject.Object;

	ConstructorHelpers::FObjectFinder<UInputAction> LookObject(
		TEXT("/Script/EnhancedInput.InputAction'/Game/GameContent/Blueprints/Inputs/IA_Look.IA_Look'")
	);

	LookAction = LookObject.Object;
}

// ==================== Lifecycles ==================== //

void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void APickup::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInput->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInput->BindAction(MoveAction.LoadSynchronous(), ETriggerEvent::Completed, this, &ThisClass::Move);
		EnhancedInput->BindAction(HandbrakeAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Handbrake);
		EnhancedInput->BindAction(HandbrakeAction.LoadSynchronous(), ETriggerEvent::Completed, this, &ThisClass::Handbrake);
		EnhancedInput->BindAction(LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInput->BindAction(ChangeModeAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ThisClass::ChangeMode);
	}
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraFollow(DeltaTime);
}

// ==================== Input ==================== //

void APickup::Move(const FInputActionValue& InputValue)
{
	const FVector2D Value = InputValue.Get<FVector2D>();

	// W Keyboard
	Movement->SetThrottleInput(Value.Y > 0.f ? 1.f : 0);

	// S Keyboard
	Movement->SetBrakeInput(Value.Y < 0.f ? 1.f : 0);

	// A and D Keyboard
	Movement->SetSteeringInput(Value.X);
}

void APickup::Handbrake(const FInputActionValue& InputValue)
{
	const bool Value = InputValue.Get<bool>();

	Movement->SetHandbrakeInput(Value);
}

void APickup::Look(const FInputActionValue& InputValue)
{
	// Make a timer so the auto camera follow is deactivated
	GetWorldTimerManager().SetTimer(CameraFollowTimerHandle, CameraFollowTimer, false);

	// Getting the value from input
	const float Value = InputValue.Get<float>();

	// Initialize some needed components
	USpringArmComponent* AdjustedSpringArm;
	FVector VehicleVector;
	float Limit;

	// Assign the value depends on the mode
	switch (VehicleMode)
	{
	case EVehicleMode::EVM_Drive:
		AdjustedSpringArm = DriveSpringArm;
		Limit 			  = DriveAngleLimit;
		VehicleVector	  = FVector::ForwardVector;
		break;

	case EVehicleMode::EVM_Throw:
		AdjustedSpringArm = ThrowSpringArm;
		Limit 			  = ThrowAngleLimit;
		VehicleVector	  = FVector::ForwardVector * -1.f;
		break;
	
	default:
		AdjustedSpringArm = DriveSpringArm;
		Limit 			  = DriveAngleLimit;
		VehicleVector	  = FVector::ForwardVector;
		break;
	}

	// Add the relative rotation by the angle
	float Angle    = GetCameraAngle(AdjustedSpringArm->GetForwardVector(), VehicleVector);
	float NewAngle = FMath::Clamp(Angle + Value, -Limit, Limit);

	// Finally add angle to the vehicle vector rotation
	if (NewAngle != -Limit || NewAngle != Limit)
		AdjustedSpringArm->AddLocalRotation(FRotator(0.f, Value, 0.f));
}

void APickup::ChangeMode(const FInputActionValue& InputValue)
{
	int8 Value = static_cast<int8>(InputValue.Get<float>());
	int8 Current = (int8)VehicleMode;

	// clamp to the min once exceeding the max and vice versa
	VehicleMode = Current + Value > 2 ? EVehicleMode::EVM_Drive :
				  Current + Value < 0 ? EVehicleMode::EVM_Both  : 
				  (EVehicleMode)(Current + Value);

	// Change the camera depends on the vehicle mode
	DriveCamera->Deactivate();
	ThrowCamera->Deactivate();

	switch (VehicleMode)
	{
	case EVehicleMode::EVM_Drive:
		DriveCamera->Activate();
		break;
	
	case EVehicleMode::EVM_Throw:
		ThrowCamera->Activate();
		break;

	default:
		DriveCamera->Activate();
		break;
	}
}

// ==================== Camera ==================== //

float APickup::GetCameraAngle(const FVector& CameraVector, const FVector& VehicleVector)
{
	// We just care about 2D direction, so just to make sure make it normalized 2D
	// NOTE: VehicleVector doesn't need to be normalized as its already a normalized 2D
	FVector Normalized2D = CameraVector.GetSafeNormal2D();

	// Using dot product to get the angle
	float CosX  = FVector::DotProduct(VehicleVector, Normalized2D);
	float Angle = FMath  ::Acos(CosX) * 180.f / PI; /* Convert it directly to degree */

	// Get the direction using cross product
	const FVector Cross = FVector::CrossProduct(VehicleVector, Normalized2D);
	Angle = Cross.Z < 0 ? Angle * -1.f : Angle;

	return Angle;
}

void APickup::CameraFollow(float DeltaTime)
{
	// Only following when the timer is over
	bool bActivated = !GetWorldTimerManager().IsTimerActive(CameraFollowTimerHandle) || Movement->GetForwardSpeedMPH() > 1.f;
	
	if (!bActivated) return;

	USpringArmComponent* AdjustedSpringArm;
	float VehicleVector;

	switch (VehicleMode)
	{
		case EVehicleMode::EVM_Drive:
			AdjustedSpringArm = DriveSpringArm;
			VehicleVector = 0.f;
			break;

		case EVehicleMode::EVM_Throw:
			AdjustedSpringArm = ThrowSpringArm;
			VehicleVector = 180.f;
			break;

		default:
			AdjustedSpringArm = DriveSpringArm;
			VehicleVector = 0.f;
			break;
	}

	float CurrentYaw = AdjustedSpringArm->GetRelativeRotation().Yaw;
	float NewYaw	 = FMath::FInterpTo(CurrentYaw, VehicleVector, DeltaTime, 1.f);

	AdjustedSpringArm->SetRelativeRotation(FRotator(0.f, NewYaw, 0.f));
}
