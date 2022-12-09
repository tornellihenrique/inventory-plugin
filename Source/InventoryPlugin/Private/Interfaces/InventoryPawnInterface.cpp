// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/InventoryPawnInterface.h"

#include "Templates/SubclassOf.h"

bool IInventoryPawnInterface::OnContainerInteract_Implementation(UContainerComponent* TargetContainer)
{
	return true;
}

bool IInventoryPawnInterface::TryAddNewItem_Implementation(class UContainerComponent* TargetContainer, TSubclassOf<class AActor> ItemClass, const int32 Quantity)
{
	return true;
}

bool IInventoryPawnInterface::TryAddNewItemFromTemplate_Implementation(class UContainerComponent* TargetContainer, AActor* ItemTemplate, const int32 Quantity)
{
	return true;
}

bool IInventoryPawnInterface::TryAddItem_Implementation(class UContainerComponent* TargetContainer, AActor* TargetItem)
{
	return true;
}

bool IInventoryPawnInterface::TryMoveItem_Implementation(class UContainerComponent* TargetContainer, AActor* TargetItem)
{
	return true;
}

bool IInventoryPawnInterface::TryAddNewItemToIndex_Implementation(class UInventoryComponent* TargetContainer, TSubclassOf<AActor> ItemClass, const int32 Index, const int32 Quantity)
{
	return true;
}

bool IInventoryPawnInterface::TryAddNewItemFromTemplateToIndex_Implementation(class UInventoryComponent* TargetContainer, AActor* ItemTemplate, const int32 Index, const int32 Quantity)
{
	return true;
}

bool IInventoryPawnInterface::TryAddItemToIndex_Implementation(class UInventoryComponent* TargetContainer, AActor* TargetItem, const int32 Index)
{
	return true;
}

bool IInventoryPawnInterface::TryMoveItemToIndex_Implementation(class UInventoryComponent* TargetContainer, AActor* TargetItem, const int32 Index)
{
	return true;
}
