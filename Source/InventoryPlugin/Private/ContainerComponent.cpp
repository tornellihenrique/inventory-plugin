// Fill out your copyright notice in the Description page of Project Settings.


#include "ContainerComponent.h"

#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "Interfaces/ItemInterface.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "Container"

UContainerComponent::UContainerComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void UContainerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Items);
}

void UContainerComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UContainerComponent::InitializeContainerWithItems_Implementation(const TArray<class AActor*>& NewItems)
{
	for (const auto& NewItem : NewItems)
		InternalTryAddItem(NewItem);
	
	return true;
}

bool UContainerComponent::InitializeContainerWithNewItems_Implementation(const TArray<TSubclassOf<AActor>>& NewItemClasses)
{
	for (const auto& NewItemClass : NewItemClasses)
		InternalTryAddNewItem(NewItemClass);
	
	return true;
}

bool UContainerComponent::CanAddItem_Implementation(const AActor* TargetItem) const
{
	return true;
}

bool UContainerComponent::CanAddItemAtIndex_Implementation(const AActor* TargetItem, const int32 Index, const bool bCheckStacking) const
{
	return true;
}

bool UContainerComponent::CanMoveItem_Implementation(const AActor* TargetItem) const
{
	// if (IItemInterface::Execute_GetOwningContainerComponent(TargetItem) == this) return true;
	return CanAddItem(TargetItem);
}

bool UContainerComponent::CanMoveItemToIndex_Implementation(const AActor* TargetItem, const int32 Index) const
{
	// if (IItemInterface::Execute_GetOwningContainerComponent(TargetItem) == this) return true;
	return CanAddItemAtIndex(TargetItem, Index);
}

int32 UContainerComponent::FindItemIndex_Implementation(const AActor* TargetItem) const
{
	if (!TargetItem) return -1;
	
	for (int32 Index = 0; Index != Items.Num(); ++Index)
		if (Items[Index] == TargetItem) return Index;

	return -1;
}

FItemAddResult UContainerComponent::InternalTryAddNewItem_Implementation(TSubclassOf<AActor> ItemClass, const int32 Quantity/* = -1*/)
{
	ensure(ItemClass);
	ensure(GetOwner());

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	FTransform SpawnTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation());
	
	if (AActor* NewItem = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnTransform, SpawnParameters))
		if (NewItem->Implements<UItemInterface>())
			if (IItemInterface::Execute_Initialize(NewItem, true))
			{
				if (Quantity != -1)
					if (IItemInterface::Execute_SetQuantity(NewItem, Quantity))
						return InternalTryAddItem(NewItem);

				return InternalTryAddItem(NewItem);
			}

	return FItemAddResult::AddedNone(-1, FText::Format(LOCTEXT("UnknowError1", "Couldn't add a Item {0}."), FText::FromString(ItemClass->GetName())));
}

FItemAddResult UContainerComponent::InternalTryAddNewItemFromTemplate_Implementation(AActor* ItemTemplate, const int32 Quantity)
{
	ensure(GetOwner());
	
	if (!ItemTemplate) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError2", "Couldn't add the Item null."));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Template = ItemTemplate;
	
	FTransform SpawnTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation());
	
	if (AActor* NewItem = GetWorld()->SpawnActor<AActor>(ItemTemplate->GetClass(), SpawnTransform, SpawnParameters))
		if (NewItem->Implements<UItemInterface>())
			if (IItemInterface::Execute_Initialize(NewItem, true))
			{
				if (Quantity != -1)
					if (IItemInterface::Execute_SetQuantity(NewItem, Quantity))
						return InternalTryAddItem(NewItem);

				return InternalTryAddItem(NewItem);
			}

	return FItemAddResult::AddedNone(-1, FText::Format(LOCTEXT("UnknowError1", "Couldn't add a Item {0}."), FText::FromString(ItemTemplate->GetName())));
}

FItemAddResult UContainerComponent::InternalTryAddItem_Implementation(AActor* TargetItem)
{
	if (!TargetItem) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError2", "Couldn't add the Item null."));
	if (!CanAddItem(TargetItem)) return FItemAddResult::AddedNone(0, LOCTEXT("NotEnouthSpace", "Couldn't add the Item, no space left."));

	if (TargetItem->Implements<UItemInterface>())
		if (IItemInterface::Execute_OnAddedToContainer(TargetItem, this))
		{
			const FItemAddResult Result = InternalAddItem(TargetItem);
			OnRep_Items();

			return Result;
		}

	return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError3", "Item not configured."));
}

FItemAddResult UContainerComponent::InternalAddItem_Implementation(AActor* TargetItem)
{
	Items.Add(TargetItem);
	return FItemAddResult::AddedAll(IItemInterface::Execute_GetQuantity(TargetItem));
}

int32 UContainerComponent::InternalConsumeItem_Implementation(AActor* TargetItem, const int32 Quantity)
{
	if (!TargetItem) return 0;
	if (!TargetItem->Implements<UItemInterface>()) return 0;

	const int32 ItemQuantity = IItemInterface::Execute_GetQuantity(TargetItem);
	const int32 RemoveQuantity = Quantity == -1 ? ItemQuantity : FMath::Min(Quantity, ItemQuantity);

	ensure(!(ItemQuantity - RemoveQuantity < 0));

	if (ItemQuantity - RemoveQuantity <= 0)
	{
		InternalRemoveItemReferences(TargetItem);
		OnRep_Items();

		TargetItem->Destroy();
	}
	else
	{
		IItemInterface::Execute_SetQuantity(TargetItem, ItemQuantity - RemoveQuantity);
	}

	return RemoveQuantity;
}

bool UContainerComponent::InternalRemoveItemReferences_Implementation(AActor* TargetItem)
{
	if (!TargetItem) return false;

	for (int32 Index = 0; Index != Items.Num(); ++Index)
		if (Items[Index] == TargetItem)
			Items[Index] = nullptr;

	return true;
}

FItemAddResult UContainerComponent::InternalTryMoveItem_Implementation(AActor* TargetItem)
{
	if (!TargetItem) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError2", "Couldn't add the Item null."));
	if (!TargetItem->Implements<UItemInterface>()) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError3", "Item not configured."));
	if (!CanMoveItem(TargetItem)) return FItemAddResult::AddedNone(0, LOCTEXT("NotEnouthSpace", "Couldn't add the Item, no space left."));

	if (IItemInterface::Execute_OnAddedToContainer(TargetItem, this))
	{
		const FItemAddResult Result = InternalMoveItem(TargetItem);
		OnRep_Items();

		return Result;
	}

	return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError3", "Item not configured."));
}

FItemAddResult UContainerComponent::InternalMoveItem_Implementation(AActor* TargetItem) 
{
	class UContainerComponent* ContainerComponent = IItemInterface::Execute_GetOwningContainerComponent(TargetItem);
	
	if (ContainerComponent) ContainerComponent->InternalConsumeItem(TargetItem);
	
	const FItemAddResult Result = InternalTryAddNewItemFromTemplate(TargetItem);

	if (Result.Result == EItemAddResult::IAR_AllItemsAdded)
		if (!ContainerComponent) TargetItem->Destroy();

	return Result;
}

void UContainerComponent::OnRep_Items()
{
	OnContainerUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE