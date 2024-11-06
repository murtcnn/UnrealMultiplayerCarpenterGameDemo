// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

#include "Carpenter/Interactables/Furniture.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Carpenter/Structs/FurniturePropertiesStruct.h"
#include "Carpenter/UI/OrderUI.h"

AMyGameState::AMyGameState()
{
	// Enable replication for this GameState
	bReplicates = true;
}

void AMyGameState::BeginPlay()
{
	Super::BeginPlay();
	MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD());

	Money = InitialMoney;

	CreateRandomOrder();
}

void AMyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyGameState, Money);
	DOREPLIFETIME(AMyGameState, CurrentOrderMaterial);
	DOREPLIFETIME(AMyGameState, CurrentOrderMesh);
}

void AMyGameState::SpendMoney(float Amount, bool& Success)
{
	Success = false;
	
	if (Amount <= Money)
	{
		Money -= Amount;
		Success = true;

		// Broadcast that money has been spent.
		if (OnMoneyUpdated.IsBound())
		{
			OnMoneyUpdated.Broadcast();
		}
	}
}

void AMyGameState::EarnMoney(float Amount)
{
	Money+=Amount;

	// Broadcast that money has been earned.
	if (OnMoneyUpdated.IsBound())
	{
		OnMoneyUpdated.Broadcast();
	}
}

float AMyGameState::GetCurrentMoney() const
{
	return Money;
}

void AMyGameState::CreateRandomOrder()
{
	if (!FurniturePropertiesDataTable) return;

	if (HasAuthority())
	{
		TArray<FName> RowNames = FurniturePropertiesDataTable->GetRowNames();
		
		if (RowNames.Num() == 0) return;

		FName RandomRowName = RowNames[FMath::RandRange(0, RowNames.Num() - 1)];
		FFurniturePropertiesStruct* RandomRow = FurniturePropertiesDataTable->FindRow<FFurniturePropertiesStruct>(RandomRowName, TEXT(""));
		CurrentOrderMesh = RandomRow->FurnitureMesh;
		CurrentOrderMaterial = RandomRow->FurnitureMaterial;
		ForceNetUpdate();
	}
	

	if (MyHUD)
	{
		MyHUD->TurnOnOrderUI();
		OrderUI = MyHUD->OrderUI;
		OrderUI->UpdateOrderTexts(FText::FromString(CurrentOrderMesh->GetName()), FText::FromString(CurrentOrderMaterial->GetName()));
	}
}

void AMyGameState::GetCurrentOrder(UStaticMesh*& OrderMesh, UMaterialInterface*& OrderMaterial) const
{
	if (!CurrentOrderMesh || !CurrentOrderMaterial) return;

	OrderMesh = CurrentOrderMesh;
	OrderMaterial = CurrentOrderMaterial;
}

void AMyGameState::CheckPlacedOrder(AFurniture* PlacedFurniture)
{
	UStaticMesh* OrderMesh = PlacedFurniture->GetFurnitureMesh();
	UMaterialInterface* OrderMaterial = PlacedFurniture->GetFurnitureMaterial();
	if (OrderMesh == CurrentOrderMesh && OrderMaterial == CurrentOrderMaterial)
	{
		RemoveCompletedOrder(PlacedFurniture);
		CreateRandomOrder();
	}
}

void AMyGameState::RemoveCompletedOrder(AFurniture* PlacedFurniture)
{
	EarnMoney(PlacedFurniture->Price);
	PlacedFurniture->GetDelivered();
	MyHUD->TurnOffOrderUI();
}

void AMyGameState::OnRep_CurrentOrderMesh()
{
	if (OrderUI)
	{
		OrderUI->UpdateOrderTexts(FText::FromString(CurrentOrderMesh->GetName()), FText::FromString(CurrentOrderMaterial->GetName()));
	}
}

void AMyGameState::OnRep_CurrentOrderMaterial()
{
	if (OrderUI)
	{
		OrderUI->UpdateOrderTexts(FText::FromString(CurrentOrderMesh->GetName()), FText::FromString(CurrentOrderMaterial->GetName()));
	}
}




