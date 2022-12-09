// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryPawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryPawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYPLUGIN_API IInventoryPawnInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn")
	bool OnContainerInteract(class UContainerComponent* TargetContainer);
	virtual bool OnContainerInteract_Implementation(class UContainerComponent* TargetContainer);

	//////////////////////////////////////////////
	/// Container API
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn", meta=(AutoCreateRefTerm="Quantity"))
	bool TryAddNewItem(class UContainerComponent* TargetContainer, TSubclassOf<class AActor> ItemClass, const int32 Quantity = -1);
	virtual bool TryAddNewItem_Implementation(class UContainerComponent* TargetContainer, TSubclassOf<class AActor> ItemClass, const int32 Quantity = -1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn", meta=(AutoCreateRefTerm="Quantity"))
	bool TryAddNewItemFromTemplate(class UContainerComponent* TargetContainer, class AActor* ItemTemplate, const int32 Quantity = -1);
	virtual bool TryAddNewItemFromTemplate_Implementation(class UContainerComponent* TargetContainer, class AActor* ItemTemplate, const int32 Quantity = -1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn")
	bool TryAddItem(class UContainerComponent* TargetContainer, class AActor* TargetItem);
	virtual bool TryAddItem_Implementation(class UContainerComponent* TargetContainer, class AActor* TargetItem);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn")
	bool TryMoveItem(class UContainerComponent* TargetContainer, class AActor* TargetItem);
	virtual bool TryMoveItem_Implementation(class UContainerComponent* TargetContainer, class AActor* TargetItem);

	//////////////////////////////////////////////
	/// Inventory API

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn", meta=(AutoCreateRefTerm="Quantity"))
	bool TryAddNewItemToIndex(class UInventoryComponent* TargetContainer, TSubclassOf<class AActor> ItemClass, const int32 Index, const int32 Quantity = -1);
	virtual bool TryAddNewItemToIndex_Implementation(class UInventoryComponent* TargetContainer, TSubclassOf<class AActor> ItemClass, const int32 Index, const int32 Quantity = -1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn", meta=(AutoCreateRefTerm="Quantity"))
	bool TryAddNewItemFromTemplateToIndex(class UInventoryComponent* TargetContainer, class AActor* ItemTemplate, const int32 Index, const int32 Quantity = -1);
	virtual bool TryAddNewItemFromTemplateToIndex_Implementation(class UInventoryComponent* TargetContainer, class AActor* ItemTemplate, const int32 Index, const int32 Quantity = -1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn")
	bool TryAddItemToIndex(class UInventoryComponent* TargetContainer, class AActor* TargetItem, const int32 Index);
	virtual bool TryAddItemToIndex_Implementation(class UInventoryComponent* TargetContainer, class AActor* TargetItem, const int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Pawn")
	bool TryMoveItemToIndex(class UInventoryComponent* TargetContainer, class AActor* TargetItem, const int32 Index);
	virtual bool TryMoveItemToIndex_Implementation(class UInventoryComponent* TargetContainer, class AActor* TargetItem, const int32 Index);
	
};
