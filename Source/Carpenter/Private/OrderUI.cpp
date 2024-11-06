// Fill out your copyright notice in the Description page of Project Settings.


#include "Carpenter/Public/OrderUI.h"

#include "Components/TextBlock.h"

void UOrderUI::UpdateOrderTexts(FText MeshName, FText MaterialName)
{
	TXT_OrderMesh->SetText(MeshName);
	TXT_OrderMaterial->SetText(MaterialName);
}
