// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrderUI.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class CARPENTER_API UOrderUI : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_OrderMesh;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TXT_OrderMaterial;

public:
	UFUNCTION()
	void UpdateOrderTexts(FText MeshName, FText MaterialName);
	
};
