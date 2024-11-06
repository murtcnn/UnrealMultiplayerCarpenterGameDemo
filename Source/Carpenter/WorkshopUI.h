// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameState.h"
#include "Blueprint/UserWidget.h"
#include "WorkshopUI.generated.h"

class UProgressBar;
class UTextBlock;
class UButton;
class AWorkshop;
/**
 * 
 */
UCLASS()
class CARPENTER_API UWorkshopUI : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY()
	AMyGameState* MyGameState;

public:
	
	UFUNCTION()
	void UpdateCreationProgressBar(float Percent);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;


private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> BAR_CreationProgress = nullptr;
};
