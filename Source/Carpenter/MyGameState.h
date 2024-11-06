// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MyGameState.generated.h"

class UOrderUI;
class AMyPlayerController;
class AMyHUD;
struct FFurniturePropertiesStruct;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoneySpent);

UCLASS()
class CARPENTER_API AMyGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMyGameState();
	
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Money
	UFUNCTION(BlueprintCallable)
	void SpendMoney(float Amount, bool& Success);

	UFUNCTION()
	void EarnMoney(float Amount);

	UFUNCTION()
	float GetCurrentMoney() const;

	UPROPERTY(BlueprintAssignable)
	FOnMoneySpent OnMoneyUpdated;

	// Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data Table")
	UDataTable* FurniturePropertiesDataTable;
	
	UFUNCTION(BlueprintCallable)
	void GetCurrentOrder(UStaticMesh*& OrderMesh, UMaterialInterface*& OrderMaterial) const;

	UFUNCTION()
	void CheckPlacedOrder(AFurniture* PlacedFurniture);
	
protected:
	// Data
	UPROPERTY()
	AMyHUD* MyHUD;

	UPROPERTY()
	AMyPlayerController* MyPlayerController;

	UPROPERTY()
	UOrderUI* OrderUI;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	float Money;

	UFUNCTION(BlueprintCallable)
	void CreateRandomOrder();



	UFUNCTION()
	void RemoveCompletedOrder(AFurniture* PlacedFurniture);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentOrderMesh, BlueprintReadOnly)
	UStaticMesh* CurrentOrderMesh;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentOrderMaterial, BlueprintReadOnly)
	UMaterialInterface* CurrentOrderMaterial;

	UFUNCTION()
	void OnRep_CurrentOrderMesh();

	UFUNCTION()
	void OnRep_CurrentOrderMaterial();
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InitialMoney;
};
