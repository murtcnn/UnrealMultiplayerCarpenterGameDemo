// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Workshop_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWorkshop_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CARPENTER_API IWorkshop_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Workshop")
	void Auth_CreateFurniture();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Workshop")
	void Auth_CheckShouldCreate();
	
};
