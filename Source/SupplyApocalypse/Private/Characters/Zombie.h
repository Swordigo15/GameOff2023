// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class SUPPLYAPOCALYPSE_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	AZombie();

	// ===== Lifecycles ========== //

	virtual void Tick(float DeltaTime) override;

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

};
