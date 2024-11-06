// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkshopUI.h"

#include "MyGameState.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"


void UWorkshopUI::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWorkshopUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
}

void UWorkshopUI::UpdateCreationProgressBar(float Percent)
{
	BAR_CreationProgress->SetPercent(Percent);
}

