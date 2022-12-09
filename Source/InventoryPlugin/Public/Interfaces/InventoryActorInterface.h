// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYPLUGIN_API IInventoryActorInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Actor")
	const TArray<class UInventoryComponent*> GetInventoryList() const;
	virtual const TArray<class UInventoryComponent*> GetInventoryList_Implementation() const;
	
};
