// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/InventoryFunctionLibrary.h"

#include "GameFramework/Actor.h"
#include "Interfaces/ItemInterface.h"

FIntPoint UInventoryFunctionLibrary::IndexToTile(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

int32 UInventoryFunctionLibrary::TileToIndex(const FIntPoint Tile, const int32 Columns)
{
	return Tile.X + (Tile.Y * Columns);
}

bool UInventoryFunctionLibrary::GetDraggingItemPosition(const FVector2D& MousePosition, const float TileSize, const AActor* TargetItem, FIntPoint& DraggingItemPosition)
{
	if (TargetItem && TargetItem->Implements<UItemInterface>())
	{
		const FIntPoint Dimensions = IItemInterface::Execute_GetBasicInfo(TargetItem).Size;

		const int32 X = MousePosition.X;
		const int32 Y = MousePosition.Y;

		const int32 ConvTileSize = TileSize;

		const bool bRight = X % ConvTileSize > TileSize / 2;
		const bool bDown = Y % ConvTileSize > TileSize / 2;

		const int32 TargetX = Dimensions.X - (bRight ? 1 : 0);
		const int32 TargetY = Dimensions.Y - (bDown ? 1 : 0);
	
		const FIntPoint TargetCoord(FMath::Clamp(TargetX, 0, TargetX), FMath::Clamp(TargetY, 0, TargetY));

		const FVector2D Aux = (MousePosition / TileSize);

		DraggingItemPosition = FIntPoint(FMath::TruncToInt(Aux.X), FMath::TruncToInt(Aux.Y)) - (TargetCoord / 2);

		return true;
	}

	DraggingItemPosition = FIntPoint();
	
	return false;
}
