// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ItemInterface.h"

#define LOCTEXT_NAMESPACE "Inventory"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	Items.SetNum(GetCapacity());
	OnRep_Items();
}

bool UInventoryComponent::CanAddItem_Implementation(const AActor* TargetItem) const
{
	if (!TargetItem) return false;
	if (!TargetItem->Implements<UItemInterface>()) return false;

	// Can't add the Item to its own Inventory
	if (const UInventoryComponent* TargetItemInventory = Cast<UInventoryComponent>(TargetItem->GetComponentByClass(GetClass())))
		if (TargetItemInventory == this) return false;

	if (!ItemsFilter.IsEmpty() && !IsItemCompatible(TargetItem)) return false;

	// Can't add if exceed max weight
	if (IItemInterface::Execute_GetBasicInfo(TargetItem).Weight + GetCurrentWeight() > WeightCapacity) return false;
	
	for (int32 Index = 0; Index < Items.Num(); Index++) if (CanAddItemAtIndex(TargetItem, Index)) return true;

	return false;
}

bool UInventoryComponent::CanAddItemAtIndex_Implementation(const AActor* TargetItem, const int32 Index, const bool bCheckStacking/* = true*/) const
{
	if (!TargetItem) return false;
	if (!Items.IsValidIndex(Index)) return false;
	if (!TargetItem->Implements<UItemInterface>()) return false;

	// Can't add the Item to its own Inventory
	if (const UInventoryComponent* TargetItemInventory = Cast<UInventoryComponent>(TargetItem->GetComponentByClass(GetClass())))
		if (TargetItemInventory == this) return false;

	if (!ItemsFilter.IsEmpty() && !IsItemCompatible(TargetItem)) return false;

	// Can't add if exceed max weight
	if (IItemInterface::Execute_GetBasicInfo(TargetItem).Weight + GetCurrentWeight() > WeightCapacity) return false;

	if (const AActor* InvItem = Items[Index])
	{
		if (InvItem == TargetItem) return true;

		if (bCheckStacking)
			if (TargetItem->Implements<UItemInterface>())
				if (InvItem->GetClass() == TargetItem->GetClass() && !IItemInterface::Execute_GetIsStackFull(InvItem))
					return true;
	}
	else return true;

	return false;
}

bool UInventoryComponent::CanMoveItem_Implementation(const AActor* TargetItem) const
{
	return Super::CanMoveItem_Implementation(TargetItem);
}

bool UInventoryComponent::CanMoveItemToIndex_Implementation(const AActor* TargetItem, const int32 Index) const
{
	return Super::CanMoveItemToIndex_Implementation(TargetItem, Index);
}

int32 UInventoryComponent::GetCapacity_Implementation() const
{
	return 0;
}

float UInventoryComponent::GetCurrentWeight() const
{
	float Weight = 0.f;

	TSet<AActor*> Seen;

	for (auto& Item : Items)
	{
		if (Item && !Seen.Contains(Item) && Item->Implements<UItemInterface>())
		{
			Weight += IItemInterface::Execute_GetStackWeight(Item);

			Seen.Add(Item);
		}
	}

	return Weight;
}

bool UInventoryComponent::IsItemCompatible(const class AActor* TargetItem) const
{
	if (!TargetItem) return false;
	for (const auto& ItemFilter : ItemsFilter) if (TargetItem->GetClass()->IsChildOf(ItemFilter)) return true;
	return false;
}

FItemAddResult UInventoryComponent::InternalTryAddNewItemToIndex_Implementation(TSubclassOf<AActor> ItemClass, const int32 Index, const int32 Quantity)
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
						return InternalTryAddItemToIndex(NewItem, Index);

				return InternalTryAddItemToIndex(NewItem, Index);
			}

	return FItemAddResult::AddedNone(-1, FText::Format(LOCTEXT("UnknowError1", "Couldn't add a Item {0}."), FText::FromString(ItemClass->GetName())));
}

FItemAddResult UInventoryComponent::InternalTryAddNewItemFromTemplateToIndex_Implementation(AActor* ItemTemplate, const int32 Index, const int32 Quantity)
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
						return InternalTryAddItemToIndex(NewItem, Index);

				return InternalTryAddItemToIndex(NewItem, Index);
			}

	return FItemAddResult::AddedNone(-1, FText::Format(LOCTEXT("UnknowError1", "Couldn't add a Item {0}."), FText::FromString(ItemTemplate->GetName())));
}

FItemAddResult UInventoryComponent::InternalTryAddItemToIndex_Implementation(AActor* TargetItem, const int32 Index)
{
	if (!TargetItem) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError2", "Couldn't add the Item null."));
	if (!CanAddItemAtIndex(TargetItem, Index)) return FItemAddResult::AddedNone(0, LOCTEXT("NotEnouthSpace", "Couldn't add the Item, no space left."));

	if (TargetItem->Implements<UItemInterface>())
		if (IItemInterface::Execute_OnAddedToContainer(TargetItem, this))
		{
			const FItemAddResult Result = InternalAddItemAtIndex(TargetItem, Index);
			OnRep_Items();

			return Result;
		}

	return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError3", "Item not configured."));
}

FItemAddResult UInventoryComponent::InternalTryMoveItemToIndex_Implementation(AActor* TargetItem, const int32 Index)
{
	if (!TargetItem) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError2", "Couldn't add the Item null."));
	if (!TargetItem->Implements<UItemInterface>()) return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError3", "Item not configured."));
	if (!CanMoveItemToIndex(TargetItem, Index)) return FItemAddResult::AddedNone(0, LOCTEXT("NotEnouthSpace", "Couldn't add the Item, no space left."));

	if (IItemInterface::Execute_OnAddedToContainer(TargetItem, this))
	{
		const FItemAddResult Result = InternalMoveItemToIndex(TargetItem, Index);
		OnRep_Items();

		return Result;
	}

	return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError3", "Item not configured."));
}

FItemAddResult UInventoryComponent::InternalAddItem_Implementation(AActor* TargetItem)
{
	FItemAddResult Result = FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError1", "Coudn't add the Item."));

	if (!TargetItem) return Result;

	for (int32 Index = 0; Index < Items.Num(); Index++)
		if (Result = InternalAddItemAtIndex(TargetItem, Index); Result.Result == EItemAddResult::IAR_AllItemsAdded)
			return Result;

	return Result;
}

FItemAddResult UInventoryComponent::InternalMoveItem_Implementation(AActor* TargetItem)
{
	return Super::InternalMoveItem_Implementation(TargetItem);
}

FItemAddResult UInventoryComponent::InternalAddItemAtIndex_Implementation(AActor* TargetItem, const int32 Index)
{
	if (CanAddItemAtIndex(TargetItem, Index))
	{
		const float TargetItemWeight = IItemInterface::Execute_GetBasicInfo(TargetItem).Weight;
		const float TargetItemQuantity = IItemInterface::Execute_GetQuantity(TargetItem);
	
		if (AActor* InvItem = Items[Index])
		{
			const float InvItemMaxStackSize = IItemInterface::Execute_GetBasicInfo(InvItem).MaxStackSize;
			const float InvItemQuantity = IItemInterface::Execute_GetQuantity(InvItem);
			
			const int32 WeightMaxAddAmount = FMath::IsNearlyZero(TargetItemWeight) ? TargetItemQuantity : FMath::FloorToInt((GetWeightCapacity() - GetCurrentWeight()) / TargetItemWeight);
			const int32 QuantityMaxAddAmount = FMath::Min(InvItemMaxStackSize - InvItemQuantity, TargetItemQuantity);
			const int32 AddAmount = FMath::Min(WeightMaxAddAmount, QuantityMaxAddAmount);

			if (AddAmount > 0)
			{
				IItemInterface::Execute_SetQuantity(InvItem, InvItemQuantity + AddAmount);

				if (AddAmount >= TargetItemQuantity) return FItemAddResult::AddedAll(AddAmount);

				IItemInterface::Execute_SetQuantity(TargetItem, TargetItemQuantity - AddAmount);

				return FItemAddResult::AddedSome(TargetItemQuantity, AddAmount, FText::FromString(""));
			}

			return FItemAddResult::AddedNone(0, LOCTEXT("NotEnouthSpace", "Couldn't add the Item, no space left."));
		}

		const int32 WeightMaxAddAmount = FMath::IsNearlyZero(TargetItemWeight) ? TargetItemQuantity : FMath::FloorToInt((GetWeightCapacity() - GetCurrentWeight()) / TargetItemWeight);
		const int32 AddAmount = FMath::Min(WeightMaxAddAmount, TargetItemQuantity);

		if (AddAmount > 0)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.Owner = GetOwner();
			SpawnParameters.bNoFail = true;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParameters.Template = TargetItem;

			FTransform SpawnTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation());

			if (AActor* NewItem = GetWorld()->SpawnActor<AActor>(TargetItem->GetClass(), SpawnTransform, SpawnParameters))
				if (NewItem->Implements<UItemInterface>())
					if (IItemInterface::Execute_Initialize(NewItem, true))
						if (IItemInterface::Execute_OnAddedToContainer(NewItem, this))
						{
							InternalSetItemReferences(NewItem, Index);
							OnRep_Items();

							IItemInterface::Execute_SetQuantity(NewItem, AddAmount);

							if (AddAmount >= TargetItemQuantity) return FItemAddResult::AddedAll(AddAmount);

							IItemInterface::Execute_SetQuantity(TargetItem, TargetItemQuantity - AddAmount);

							return FItemAddResult::AddedSome(TargetItemQuantity, AddAmount, FText::FromString(""));
						}
		}
	}

	return FItemAddResult::AddedNone(-1, LOCTEXT("UnknowError1", "Coudn't add the Item."));
}

bool UInventoryComponent::InternalSetItemReferences_Implementation(AActor* TargetItem, const int32 Index)
{
	Items[Index] = TargetItem;

	return true;
}

FItemAddResult UInventoryComponent::InternalMoveItemToIndex_Implementation(AActor* TargetItem, const int32 Index)
{
	class UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(IItemInterface::Execute_GetOwningContainerComponent(TargetItem));
	
	const FItemAddResult Result = InternalTryAddNewItemFromTemplateToIndex(TargetItem, Index);

	if (Result.Result == EItemAddResult::IAR_AllItemsAdded)
	{	
		if (InventoryComponent) InventoryComponent->InternalConsumeItem(TargetItem, Result.AmountGiven);
		else TargetItem->Destroy();
	}
	else
	{
		IItemInterface::Execute_SetQuantity(TargetItem, IItemInterface::Execute_GetQuantity(TargetItem) - Result.AmountGiven);
	}
	
	return Result;
}

#undef LOCTEXT_NAMESPACE
