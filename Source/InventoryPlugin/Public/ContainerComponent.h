// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/StructEnumLibrary.h"
#include "ContainerComponent.generated.h"

//Called when the container is changed and the UI needs an update. 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContainerUpdated);

UCLASS( ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent) )
class INVENTORYPLUGIN_API UContainerComponent : public UActorComponent
{
	GENERATED_BODY()

	//////////////////////////////////////////////
	/// Base Methods

public:
	
	UContainerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	
	virtual void BeginPlay() override;

	//////////////////////////////////////////////
	/// API

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	bool InitializeContainerWithItems(const TArray<class AActor*>& NewItems);
	virtual bool InitializeContainerWithItems_Implementation(const TArray<class AActor*>& NewItems);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	bool InitializeContainerWithNewItems(const TArray<TSubclassOf<AActor>>& NewItemClasses);
	virtual bool InitializeContainerWithNewItems_Implementation(const TArray<TSubclassOf<AActor>>& NewItemClasses);

	// [RUN ONLY ON SERVER]
	// Method responsible for creating an Item and adding it to the container (use quantity = -1 for using the default value)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	FItemAddResult InternalTryAddNewItem(TSubclassOf<class AActor> ItemClass, const int32 Quantity = -1);
	virtual FItemAddResult InternalTryAddNewItem_Implementation(TSubclassOf<class AActor> ItemClass, const int32 Quantity = -1);

	// [RUN ONLY ON SERVER]
	// Method responsible for creating an Item (from another Item as template) and adding it to the container (use quantity = -1 for using the default value)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	FItemAddResult InternalTryAddNewItemFromTemplate(class AActor* ItemTemplate, const int32 Quantity = -1);
	virtual FItemAddResult InternalTryAddNewItemFromTemplate_Implementation(class AActor* ItemTemplate, const int32 Quantity = -1);

	// [RUN ONLY ON SERVER]
	// Method responsible for adding an Item to the container (ignoring the source)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	FItemAddResult InternalTryAddItem(class AActor* TargetItem);
	virtual FItemAddResult InternalTryAddItem_Implementation(class AActor* TargetItem);

	// [RUN ONLY ON SERVER]
	// Method responsible for moving the Item (removing from source and adding)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	FItemAddResult InternalTryMoveItem(class AActor* TargetItem);
	virtual FItemAddResult InternalTryMoveItem_Implementation(class AActor* TargetItem);

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Container")
	bool CanAddItem(const class AActor* TargetItem) const;
	virtual bool CanAddItem_Implementation(const class AActor* TargetItem) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Container")
	bool CanAddItemAtIndex(const class AActor* TargetItem, const int32 Index, const bool bCheckStacking = true) const;
	virtual bool CanAddItemAtIndex_Implementation(const class AActor* TargetItem, const int32 Index, const bool bCheckStacking = true) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Container")
	bool CanMoveItem(const class AActor* TargetItem) const;
	virtual bool CanMoveItem_Implementation(const class AActor* TargetItem) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Container")
	bool CanMoveItemToIndex(const class AActor* TargetItem, const int32 Index) const;
	virtual bool CanMoveItemToIndex_Implementation(const class AActor* TargetItem, const int32 Index) const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "Container")
	int32 FindItemIndex(const class AActor* TargetItem) const;
	virtual int32 FindItemIndex_Implementation(const class AActor* TargetItem) const;

	UFUNCTION(BlueprintPure, Category = "Container")
	FORCEINLINE TArray<class AActor*> GetItems() const { return Items; }
	
	UPROPERTY(BlueprintAssignable, Category = "Container")
	FOnContainerUpdated OnContainerUpdated;

	//////////////////////////////////////////////
	/// Internal

protected:

	UPROPERTY(ReplicatedUsing = OnRep_Items)
	TArray<class AActor*> Items;
	
	// [RUN ONLY ON SERVER]
	// Method responsible for actually adding the Item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	FItemAddResult InternalAddItem(class AActor* TargetItem);
	virtual FItemAddResult InternalAddItem_Implementation(class AActor* TargetItem);
	
	// [RUN ONLY ON SERVER]
	// Method responsible for consuming some Item quantity and removing it if consume all (use -1 for consume all). Returns removed amount.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	int32 InternalConsumeItem(class AActor* TargetItem, const int32 Quantity = -1);
	virtual int32 InternalConsumeItem_Implementation(class AActor* TargetItem, const int32 Quantity = -1);

	// [RUN ONLY ON SERVER]
	// Method responsible for removing the Item's references in the Items array
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	bool InternalRemoveItemReferences(class AActor* TargetItem);
	virtual bool InternalRemoveItemReferences_Implementation(class AActor* TargetItem);
	
	// [RUN ONLY ON SERVER]
	// Method responsible for actually moving the Item (removing from source and adding)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Container")
	FItemAddResult InternalMoveItem(class AActor* TargetItem);
	virtual FItemAddResult InternalMoveItem_Implementation(class AActor* TargetItem);

	//////////////////////////////////////////////
	/// OnReps
	
protected:

	UFUNCTION()
	virtual void OnRep_Items();
		
};
