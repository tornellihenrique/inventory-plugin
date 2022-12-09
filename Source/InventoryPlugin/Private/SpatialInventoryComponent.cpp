// Fill out your copyright notice in the Description page of Project Settings.


#include "SpatialInventoryComponent.h"

#include "GameFramework/Actor.h"
#include "Interfaces/ItemInterface.h"
#include "Library/InventoryFunctionLibrary.h"

TMap<AActor*, FIntPoint> USpatialInventoryComponent::GetItemsMap() const
{
	TMap<class AActor*, FIntPoint> Res;

	for (int32 Index = 0; Index < Items.Num(); Index++)
		if (AActor* Item = Items[Index])
			if (!Res.Contains(Item))
				Res.Add(Item, UInventoryFunctionLibrary::IndexToTile(Index, Columns));

	return Res;
}

bool USpatialInventoryComponent::CanAddItem_Implementation(const AActor* TargetItem) const
{
	return Super::CanAddItem_Implementation(TargetItem);
}

bool USpatialInventoryComponent::CanAddItemAtIndex_Implementation(const AActor* TargetItem, const int32 Index, const bool bCheckStacking) const
{
	if (!Super::CanAddItemAtIndex_Implementation(TargetItem, Index, bCheckStacking)) return false;

	if (const AActor* InvItem = Items[Index])
		if (const int32 InvItemIndex = FindItemIndex(InvItem); InvItemIndex == Index)
		{
			if (InvItem == TargetItem) return true;

			if (bCheckStacking)
				if (TargetItem->Implements<UItemInterface>())
					if (InvItem->GetClass() == TargetItem->GetClass() && !IItemInterface::Execute_GetIsStackFull(InvItem))
						return true;
		}

	const FIntPoint Tile = UInventoryFunctionLibrary::IndexToTile(Index, Columns);
	const FIntPoint Size = IItemInterface::Execute_GetBasicInfo(TargetItem).Size;

	for (int32 I = Tile.X; I < Tile.X + Size.X; I++)
		for (int32 J = Tile.Y; J < Tile.Y + Size.Y; J++)
		{
			const FIntPoint TargetTile(I, J);
			
			if (!IsTileValid(TargetTile)) return false;
			
			const int32 TargetTileIndex = UInventoryFunctionLibrary::TileToIndex(TargetTile, Columns);

			if (TargetTileIndex == Index) continue;

			if (!Super::CanAddItemAtIndex_Implementation(TargetItem, TargetTileIndex, false)) return false;
		}

	return true;
}

int32 USpatialInventoryComponent::GetCapacity_Implementation() const
{
	return Rows * Columns;
}

bool USpatialInventoryComponent::IsTileValid(const FIntPoint& Tile) const
{
	return Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Items.IsValidIndex(UInventoryFunctionLibrary::TileToIndex(Tile, Columns));
}

bool USpatialInventoryComponent::InternalSetItemReferences_Implementation(AActor* TargetItem, const int32 Index)
{
	const FIntPoint Tile = UInventoryFunctionLibrary::IndexToTile(Index, Columns);
	const FIntPoint Size = IItemInterface::Execute_GetBasicInfo(TargetItem).Size;
	
	for (int32 I = Tile.X; I < Tile.X + Size.X; I++)
	{
		for (int32 J = Tile.Y; J < Tile.Y + Size.Y; J++)
		{
			const int32 TargetIndex = UInventoryFunctionLibrary::TileToIndex(FIntPoint(I, J), Columns);

			Items[TargetIndex] = TargetItem;
		}
	}

	return true;
}
