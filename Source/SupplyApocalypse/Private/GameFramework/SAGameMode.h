// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SAGameMode.generated.h"

UCLASS()
class SUPPLYAPOCALYPSE_API ASAGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASAGameMode();

private:
	// ===== Initialize References ========== //

	void InitializeGameFramework();
};
