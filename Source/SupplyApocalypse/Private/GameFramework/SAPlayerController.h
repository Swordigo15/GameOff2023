// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SAPlayerController.generated.h"

class ASAHUD;
class UInputMappingContext;

UCLASS()
class SUPPLYAPOCALYPSE_API ASAPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// ===== HUDs ========== //

	FORCEINLINE void UpdateMouseCursorPosition();

	// ===== Inputs ========== //

	void UseGameInput();

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== HUDs ========== //

	UPROPERTY()
	TWeakObjectPtr<ASAHUD> SAHUD;

	// ===== Inputs ========== //

	UPROPERTY(EditAnywhere, Category=Inputs)
	TSoftObjectPtr<UInputMappingContext> GameInput;
};
