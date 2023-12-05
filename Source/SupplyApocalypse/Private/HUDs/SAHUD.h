// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SAHUD.generated.h"

UCLASS()
class ASAHUD : public AHUD
{
	GENERATED_BODY()

public:
	// ===== HUDs ========== //

	void UpdateMouseCursorPosition();

protected:
	// ===== Lifecycles ========== //

	virtual void BeginPlay() override;

private:
	// ===== HUDs ========== //

	/** Using widget mouse cursor so it won't interupt the input */
	UPROPERTY(EditAnywhere, Category=HUDs)
	TSubclassOf<UUserWidget> MouseCursorClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MouseCursor;

	void ConstructWidgets();
};
