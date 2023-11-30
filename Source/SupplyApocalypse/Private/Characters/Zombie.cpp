// Copyright Anrility. All Rights Reserved.

#include "Characters/Zombie.h"

AZombie::AZombie()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default value
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// ==================== Lifecycles ==================== //

void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
