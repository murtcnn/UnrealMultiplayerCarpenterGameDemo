// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Carpenter/UI/WorkshopUI.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UOrderUI;
/**
 * 
 */
UCLASS()
class CARPENTER_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:

	// Workshop Widget
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UWorkshopUI> WorkshopUIClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UWorkshopUI* WBP_WorkshopUI;
	
	UFUNCTION(BlueprintCallable)
	void TurnOnWorkshopWidget(AWorkshop* WorkshopActor);

	UFUNCTION(BlueprintCallable)
	void TurnOffWorkshopWidget();

	// Order Widget
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UOrderUI> OrderUIClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UOrderUI* OrderUI;

	UFUNCTION()
	void TurnOnOrderUI();

	UFUNCTION()
	void TurnOffOrderUI();
	
};
