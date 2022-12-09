// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Library/StructEnumLibrary.h"
#include "UObject/Interface.h"
#include "InventorySettingsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventorySettingsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYPLUGIN_API IInventorySettingsInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Settings")
	FInventorySettings GetInventorySettings() const;
	virtual FInventorySettings GetInventorySettings_Implementation() const = 0;
	
};
