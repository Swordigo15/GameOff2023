// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SAPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class SUPPLYAPOCALYPSE_API ASAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ===== Inputs ========== //

	void UseGameInput();

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== Inputs ========== //

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputMappingContext> GameInput;
};
