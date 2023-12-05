// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SAGameMode.generated.h"

class USupplyManager;

UCLASS()
class SUPPLYAPOCALYPSE_API ASAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASAGameMode();

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;
	
private:
	// ===== Initialize References ========== //

	void InitializeGameFramework();

	// ===== Supply Manager ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USupplyManager> SupplyManager;
};
