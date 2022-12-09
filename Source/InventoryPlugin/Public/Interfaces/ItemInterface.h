// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Library/StructEnumLibrary.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORYPLUGIN_API IItemInterface
{
	GENERATED_BODY()
	
public:

	//////////////////////////////////////////////
	/// Getters

	// Getter for Item's basic info
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	FItemBasicInfo GetBasicInfo() const;
	virtual FItemBasicInfo GetBasicInfo_Implementation() const;

	// Getter for Item's quantity
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	int32 GetQuantity() const;
	virtual int32 GetQuantity_Implementation() const;

	// Getter for Item's stack weight
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	float GetStackWeight() const;
	virtual float GetStackWeight_Implementation() const;

	// Getter for Item's stackable condition
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool GetStackable() const;
	virtual bool GetStackable_Implementation() const;

	// Getter for Item's stack full condition
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool GetIsStackFull() const;
	virtual bool GetIsStackFull_Implementation() const;

	// Getter for Item's owning container component
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	class UContainerComponent* GetOwningContainerComponent() const;
	virtual class UContainerComponent* GetOwningContainerComponent_Implementation() const;

	//////////////////////////////////////////////
	/// Setters

	// Setter for Item's quantity (should replicate)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool SetQuantity(const int32 TargetQuantity);
	virtual bool SetQuantity_Implementation(const int32 TargetQuantity);

	//////////////////////////////////////////////
	/// Item Internal
	
	// Called before the Item is created
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool Initialize(const bool bIsInContainer);
	virtual bool Initialize_Implementation(const bool bIsInContainer);
	
	// Called exactly before the Item is added to a Container Items array (used for turning off collision, visibility, etc)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool OnAddedToContainer(class UContainerComponent* TargetContainer);
	virtual bool OnAddedToContainer_Implementation(class UContainerComponent* TargetContainer);
	
	// Called exactly before the Item is added to the world (used for turning on collision, visibility, etc)
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool OnAddedToWorld();
	virtual bool OnAddedToWorld_Implementation();

	//////////////////////////////////////////////
	/// Inventory Interaction

	// Called by a Pawn when it interacts with the Item
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	bool Interact(class APawn* Instigator);
	virtual bool Interact_Implementation(class APawn* Instigator);
	
};
