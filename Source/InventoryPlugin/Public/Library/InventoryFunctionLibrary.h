// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Inventory Function Library")
	static FIntPoint IndexToTile(const int32 Index, const int32 Columns);

	UFUNCTION(BlueprintPure, Category = "Inventory Function Library")
	static int32 TileToIndex(const FIntPoint Tile, const int32 Columns);

	UFUNCTION(BlueprintPure, Category = "Inventory Function Library")
	static bool GetDraggingItemPosition(const FVector2D& MousePosition, const float TileSize, const AActor* TargetItem, FIntPoint& DraggingItemPosition);
	
};
