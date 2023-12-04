// Copyright Anrility. All Rights Reserved.

#include "HUDs/SAHUD.h"
#include "Blueprint/UserWidget.h"

// ==================== Lifecycles ==================== //

void ASAHUD::BeginPlay()
{
    Super::BeginPlay();

    ConstructWidgets();
}

// ==================== HUDs ==================== //

void ASAHUD::ConstructWidgets()
{
    APlayerController* PlayerController = GetOwningPlayerController();

    // Mouse Cursor
    MouseCursor = CreateWidget<UUserWidget>(PlayerController, MouseCursorClass);
    MouseCursor->AddToViewport();
}

void ASAHUD::UpdateMouseCursorPosition()
{
    FVector2D MousePosition;

    GetOwningPlayerController()->GetMousePosition(MousePosition.X, MousePosition.Y);

    MouseCursor->SetPositionInViewport(MousePosition);
}
