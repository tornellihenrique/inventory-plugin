// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "SpatialInventoryComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent) )
class INVENTORYPLUGIN_API USpatialInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

	//////////////////////////////////////////////
	/// Base Methods

protected:

	//////////////////////////////////////////////
	/// API

public:

	UFUNCTION(BlueprintPure, Category = "Spatial Inventory")
	TMap<class AActor*, FIntPoint> GetItemsMap() const;

	virtual bool CanAddItem_Implementation(const class AActor* TargetItem) const override;

	virtual bool CanAddItemAtIndex_Implementation(const class AActor* TargetItem, const int32 Index, const bool bCheckStacking = true) const override;
	
	//////////////////////////////////////////////
	/// Internal

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spatial Inventory")
	int32 Rows = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spatial Inventory")
	int32 Columns = 5;

	virtual int32 GetCapacity_Implementation() const override;

	virtual bool IsTileValid(const FIntPoint& Tile) const;

	virtual bool InternalSetItemReferences_Implementation(AActor* TargetItem, const int32 Index) override;

	//////////////////////////////////////////////
	/// OnReps
};
