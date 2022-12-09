#pragma once

#include "CoreMinimal.h"
#include "StructEnumLibrary.generated.h"

USTRUCT(BlueprintType)
struct FItemBasicInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Basic Info")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Basic Info")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Basic Info")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Basic Info")
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Basic Info")
	FIntPoint Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Basic Info")
	class UTexture* Thumbnail;
	
};

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemsAdded UMETA(DisplayName = "No items added"),
	IAR_SomeItemsAdded UMETA(DisplayName = "Some items added"),
	IAR_AllItemsAdded UMETA(DisplayName = "All items added")
};

//Represents the result of adding an item to the inventory.
USTRUCT(BlueprintType)
struct FItemAddResult
{

	GENERATED_BODY()

	FItemAddResult() {};
	FItemAddResult(int32 InItemQuantity) : AmountToGive(InItemQuantity), AmountGiven(0) {};
	FItemAddResult(int32 InItemQuantity, int32 InQuantityAdded) : AmountToGive(InItemQuantity), AmountGiven(InQuantityAdded) {};

	//The amount of the item that we tried to add
	UPROPERTY(BlueprintReadWrite, Category = "Item Add Result")
	int32 AmountToGive = 0;

	//The amount of the item that was actually added in the end. Maybe we tried adding 10 items, but only 8 could be added because of capacity/weight
	UPROPERTY(BlueprintReadWrite, Category = "Item Add Result")
	int32 AmountGiven = 0;

	//The result
	UPROPERTY(BlueprintReadWrite, Category = "Item Add Result")
	EItemAddResult Result = EItemAddResult::IAR_NoItemsAdded;

	//If something went wrong, like we didnt have enough capacity or carrying too much weight this contains the reason why
	UPROPERTY(BlueprintReadWrite, Category = "Item Add Result")
	FText ErrorText = FText::GetEmpty();

	//Helpers
	static FItemAddResult AddedNone(const int32 InItemQuantity, const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult(InItemQuantity);
		AddedNoneResult.Result = EItemAddResult::IAR_NoItemsAdded;
		AddedNoneResult.ErrorText = ErrorText;

		return AddedNoneResult;
	}

	static FItemAddResult AddedSome(const int32 InItemQuantity, const int32 ActualAmountGiven, const FText& ErrorText)
	{
		FItemAddResult AddedSomeResult(InItemQuantity, ActualAmountGiven);

		AddedSomeResult.Result = EItemAddResult::IAR_SomeItemsAdded;
		AddedSomeResult.ErrorText = ErrorText;

		return AddedSomeResult;
	}

	static FItemAddResult AddedAll(const int32 InItemQuantity)
	{
		FItemAddResult AddAllResult(InItemQuantity, InItemQuantity);

		AddAllResult.Result = EItemAddResult::IAR_AllItemsAdded;

		return AddAllResult;
	}

};

USTRUCT(BlueprintType)
struct FInventorySettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Settings")
	float TileSize = 50.f;
	
};