// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"
#include "FurniturePropertiesStruct.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "Furniture Properties Struct"))
struct FFurniturePropertiesStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	UStaticMesh* FurnitureMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Properties")
	UMaterialInterface* FurnitureMaterial;
};
