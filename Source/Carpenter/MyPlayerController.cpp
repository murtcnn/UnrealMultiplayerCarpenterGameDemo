// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "MyGameState.h"
#include "Workshop_Interface.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
}

void AMyPlayerController::RequestSpendMoney(float Amount, bool& Success)
{
	if (HasAuthority())
	{
		MyGameState->SpendMoney(Amount, Success);
	}
	else
	{
		Server_RequestSpendMoney(Amount);
	}
}

void AMyPlayerController::Server_CheckShouldCreate_Implementation(AActor* Workshop)
{
	if (Workshop && Workshop->GetClass()->ImplementsInterface(UWorkshop_Interface::StaticClass()))
	{
		IWorkshop_Interface::Execute_Auth_CheckShouldCreate(Workshop);
	}
}

void AMyPlayerController::Server_RequestSpendMoney_Implementation(float Amount)
{
	// MyGameState->SpendMoney(Amount, Success);
}

void AMyPlayerController::Server_CreateFurniture_Implementation(AActor* Workshop)
{
	if (Workshop && Workshop->GetClass()->ImplementsInterface(UWorkshop_Interface::StaticClass()))
	{
		IWorkshop_Interface::Execute_Auth_CreateFurniture(Workshop);
	}
}

