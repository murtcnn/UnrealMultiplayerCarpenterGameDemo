// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameState.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AMyGameState* MyGameState;
	
public:
	virtual void BeginPlay() override;
	
	// Money
	UFUNCTION(BlueprintCallable)
	void RequestSpendMoney(float Amount, bool& Success);
	
	UFUNCTION(Server, Reliable)
	void Server_RequestSpendMoney(float Amount);

	UFUNCTION(Server, Reliable)
	void Server_CreateFurniture(AActor* Workshop);

	UFUNCTION(Server, Reliable)
	void Server_CheckShouldCreate(AActor* Workshop);
	
};
