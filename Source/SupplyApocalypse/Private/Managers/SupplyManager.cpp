// Copyright Anrility. All Rights Reserved.

#include "Managers/SupplyManager.h"
#include "Kismet/GameplayStatics.h"
#include "Props/Supplies.h"
#include "Props/Supply.h"

// ==================== Lifecycles ==================== //

void USupplyManager::FindSupplies()
{
    // Get actors of supplies and supply
    AActor* SuppliesActor = UGameplayStatics::GetActorOfClass(this, ASupplies::StaticClass());
    AActor* SupplyActor   = UGameplayStatics::GetActorOfClass(this, ASupply::StaticClass());
    
    // Cast
    Supplies = Cast<ASupplies>(SuppliesActor);
    Supply   = Cast<ASupply>(SupplyActor);
}
