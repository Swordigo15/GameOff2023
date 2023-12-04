// Copyright Anrility. All Rights Reserved.

#include "GameFramework/SAGameMode.h"
#include "GameFramework/HUD.h"

ASAGameMode::ASAGameMode()
{
    InitializeGameFramework();
}

// ==================== Initialize References ==================== //

void ASAGameMode::InitializeGameFramework()
{
    // Pawn
    ConstructorHelpers::FClassFinder<APawn> PawnClass(
        TEXT("/Game/GameContent/Blueprints/Vehicles/BP_Pickup")
    );
    DefaultPawnClass = PawnClass.Class;

    // Player Controller
    ConstructorHelpers::FClassFinder<APlayerController> ControllerClass(
        TEXT("/Game/GameContent/Blueprints/GameFramework/BP_SAPlayerController")
    );
    PlayerControllerClass = ControllerClass.Class;

    // HUDs
    ConstructorHelpers::FClassFinder<AHUD> SAHUDClass(
        TEXT("/Game/GameContent/Blueprints/HUDs/BP_SAHUD")
    );
    HUDClass = SAHUDClass.Class;
}
