// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "Carpenter/UI/OrderUI.h"

// Workshop Widget
void AMyHUD::TurnOnWorkshopWidget(AWorkshop* WorkshopActor)
{
	// If the widget already exists, add it to viewport. Else create the widget.
	if (WBP_WorkshopUI)
	{
		WBP_WorkshopUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (WorkshopActor)
		{
			WBP_WorkshopUI = CreateWidget<UWorkshopUI>(GetWorld()->GetFirstPlayerController(), WorkshopUIClass);
			WBP_WorkshopUI->AddToViewport();
		}
	}
}

void AMyHUD::TurnOffWorkshopWidget()
{
	if (WBP_WorkshopUI)
	{
		WBP_WorkshopUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMyHUD::TurnOnOrderUI()
{
	if (OrderUI)
	{
		OrderUI->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		OrderUI = CreateWidget<UOrderUI>(GetWorld()->GetFirstPlayerController(), OrderUIClass);
		OrderUI->AddToViewport();
	}
}

void AMyHUD::TurnOffOrderUI()
{
	if (OrderUI)
	{
		OrderUI->SetVisibility(ESlateVisibility::Collapsed);
	}
}
