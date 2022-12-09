// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API UContainerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Container Widget")
	class UContainerComponent* ContainerComponent;
	
};
