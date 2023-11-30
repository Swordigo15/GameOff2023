// Copyright Anrility. All Rights Reserved.

#include "GameFramework/SAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// ==================== Lifecycles ==================== //

void ASAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UseGameInput();

    // Limit Pitch look
    PlayerCameraManager->ViewPitchMin = -45.f;
    PlayerCameraManager->ViewPitchMax = 25.f;
}

// ==================== Inputs ==================== //

void ASAPlayerController::UseGameInput()
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    {
        Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(GameInput.LoadSynchronous(), 0);
    }
}
