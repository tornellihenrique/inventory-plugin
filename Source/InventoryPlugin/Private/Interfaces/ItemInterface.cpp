// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/ItemInterface.h"

FItemBasicInfo IItemInterface::GetBasicInfo_Implementation() const
{
	FItemBasicInfo Value;
	Value.Name = FText::FromString("Item Name");
	Value.Description = FText::FromString("Item Description");
	Value.Weight = 0.f;
	Value.MaxStackSize = 1;
	Value.Size = FIntPoint(1, 1);
	Value.Thumbnail = nullptr;

	return Value;
}

int32 IItemInterface::GetQuantity_Implementation() const
{
	return 1;
}

float IItemInterface::GetStackWeight_Implementation() const
{
	return GetBasicInfo().Weight * GetQuantity();
}

bool IItemInterface::GetStackable_Implementation() const
{
	return GetBasicInfo().MaxStackSize > 1;
}

bool IItemInterface::GetIsStackFull_Implementation() const
{
	return GetQuantity() >= GetBasicInfo().MaxStackSize;
}

UContainerComponent* IItemInterface::GetOwningContainerComponent_Implementation() const
{
	return nullptr;
}

bool IItemInterface::SetQuantity_Implementation(const int32 TargetQuantity)
{
	return true;
}

bool IItemInterface::Initialize_Implementation(const bool bIsInContainer)
{
	return true;
}

bool IItemInterface::OnAddedToContainer_Implementation(UContainerComponent* TargetContainer)
{
	return true;
}

bool IItemInterface::OnAddedToWorld_Implementation()
{
	return true;
}

bool IItemInterface::Interact_Implementation(APawn* Instigator)
{
	// ...

	return true;
}
