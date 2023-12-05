// Copyright Anrility. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SupplyManager.generated.h"

class ASupplies;
class ASupply;

UCLASS()
class USupplyManager : public UObject
{
	GENERATED_BODY()
	
public:
	// ===== Supply Manager ========== //

	void FindSupplies();

private:
	// ===== Supply Manager ========== //

	UPROPERTY()
	TWeakObjectPtr<ASupplies> Supplies;

	UPROPERTY()
	TWeakObjectPtr<ASupply> Supply;

	// ===== Attributes ========== //

	float Stocks = 100.f;
	float CurrentStocks = Stocks;

};
