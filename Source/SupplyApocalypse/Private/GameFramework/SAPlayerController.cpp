// Copyright Anrility. All Rights Reserved.

#include "GameFramework/SAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// ==================== Lifecycles ==================== //

void ASAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UseGameInput();
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
