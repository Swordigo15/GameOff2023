// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadSpline.generated.h"

class USplineComponent;

UCLASS()
class SUPPLYAPOCALYPSE_API ARoadSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoadSpline();

	// ===== Lifecycles ========== //

	virtual void OnConstruction(const FTransform& Transform) override;

private:
	// ===== Components ========== //

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> RoadSpline;

	// ===== Assets ========== //

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> RoadMesh;
};
