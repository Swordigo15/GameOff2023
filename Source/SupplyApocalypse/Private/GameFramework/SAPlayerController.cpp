// Copyright Anrility. All Rights Reserved.

#include "GameFramework/SAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "HUDs/SAHUD.h"
#include "InputMappingContext.h"

// ==================== Lifecycles ==================== //

void ASAPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UseGameInput();

    // Initialize HUDs
    SAHUD = Cast<ASAHUD>(GetHUD());
}

// ==================== HUDs ==================== //

void ASAPlayerController::UpdateMouseCursorPosition()
{
    SAHUD->UpdateMouseCursorPosition();
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
