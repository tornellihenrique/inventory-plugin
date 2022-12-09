// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerComponent.h"
#include "InventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API UInventoryComponent : public UContainerComponent
{
	GENERATED_BODY()

	//////////////////////////////////////////////
	/// Base Methods

protected:
	
	virtual void BeginPlay() override;

	//////////////////////////////////////////////
	/// API

public:

	// [RUN ONLY ON SERVER]
	// Method responsible for creating an Item and adding it to the container (use quantity = -1 for using the default value)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	FItemAddResult InternalTryAddNewItemToIndex(TSubclassOf<class AActor> ItemClass, const int32 Index, const int32 Quantity = -1);
	virtual FItemAddResult InternalTryAddNewItemToIndex_Implementation(TSubclassOf<class AActor> ItemClass, const int32 Index, const int32 Quantity = -1);

	// [RUN ONLY ON SERVER]
	// Method responsible for creating an Item (from another Item as template) and adding it to the container (use quantity = -1 for using the default value)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	FItemAddResult InternalTryAddNewItemFromTemplateToIndex(class AActor* ItemTemplate, const int32 Index, const int32 Quantity = -1);
	virtual FItemAddResult InternalTryAddNewItemFromTemplateToIndex_Implementation(class AActor* ItemTemplate, const int32 Index, const int32 Quantity = -1);

	// [RUN ONLY ON SERVER]
	// Method responsible for adding an Item to the container (ignoring the source)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	FItemAddResult InternalTryAddItemToIndex(class AActor* TargetItem, const int32 Index);
	virtual FItemAddResult InternalTryAddItemToIndex_Implementation(class AActor* TargetItem, const int32 Index);

	// [RUN ONLY ON SERVER]
	// Method responsible for moving the Item (removing from source and adding)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	FItemAddResult InternalTryMoveItemToIndex(class AActor* TargetItem, const int32 Index);
	virtual FItemAddResult InternalTryMoveItemToIndex_Implementation(class AActor* TargetItem, const int32 Index);

	virtual bool CanAddItem_Implementation(const class AActor* TargetItem) const override;
	virtual bool CanAddItemAtIndex_Implementation(const class AActor* TargetItem, const int32 Index, const bool bCheckStacking = true) const override;

	virtual bool CanMoveItem_Implementation(const AActor* TargetItem) const override;
	virtual bool CanMoveItemToIndex_Implementation(const AActor* TargetItem, const int32 Index) const override;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	FORCEINLINE float GetWeightCapacity() const { return WeightCapacity; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Inventory")
	int32 GetCapacity() const;
	virtual int32 GetCapacity_Implementation() const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	float GetCurrentWeight() const;
	
	//////////////////////////////////////////////
	/// Internal

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float WeightCapacity = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TArray<TSubclassOf<class AActor>> ItemsFilter;

	// Checks if Item's class is present in ItemsFilter Array
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool IsItemCompatible(const class AActor* TargetItem) const;
	
	virtual FItemAddResult InternalAddItem_Implementation(AActor* TargetItem) override;

	virtual FItemAddResult InternalMoveItem_Implementation(AActor* TargetItem) override;

	// [RUN ONLY ON SERVER]
	// Method responsible for actually adding the Item to some Index
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	FItemAddResult InternalAddItemAtIndex(class AActor* TargetItem, const int32 Index);
	virtual FItemAddResult InternalAddItemAtIndex_Implementation(class AActor* TargetItem, const int32 Index);

	// [RUN ONLY ON SERVER]
	// Method responsible for setting the Item's references in the Items array
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	bool InternalSetItemReferences(class AActor* TargetItem, const int32 Index);
	virtual bool InternalSetItemReferences_Implementation(class AActor* TargetItem, const int32 Index);

	// [RUN ONLY ON SERVER]
	// Method responsible for actually moving the Item (removing from source and adding) to some Index
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	FItemAddResult InternalMoveItemToIndex(class AActor* TargetItem, const int32 Index);
	virtual FItemAddResult InternalMoveItemToIndex_Implementation(class AActor* TargetItem, const int32 Index);

	//////////////////////////////////////////////
	/// OnReps

};
