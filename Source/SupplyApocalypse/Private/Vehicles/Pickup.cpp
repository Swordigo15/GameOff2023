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

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bInheritYaw = SpringArm->bInheritPitch = SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

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
	}
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("BreakInput: %f"), Movement->GetBrakeInput());
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
