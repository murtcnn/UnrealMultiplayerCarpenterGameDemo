// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Carpenter/Interfaces/Interaction_Interface.h"
#include "GameFramework/Actor.h"
#include "Furniture.generated.h"

UCLASS()
class CARPENTER_API AFurniture : public AActor, public IInteraction_Interface
{
	GENERATED_BODY()

	// Static Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
public:	
	// Sets default values for this actor's properties
	AFurniture();

	// Ownership
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasOwner = false;

	UFUNCTION()
	void SetHasOwner(bool HasOwner);
	
	UFUNCTION()
	UMaterial* GetFurnitureMaterial() const;

	UFUNCTION(NetMulticast, Reliable)
	void SetFurnitureMaterial(UMaterial* Material);
	
	UFUNCTION()
	UStaticMesh* GetFurnitureMesh() const;

	UFUNCTION(NetMulticast, Reliable)
	void SetFurnitureMesh(UStaticMesh* FurnitureMesh);

	UFUNCTION()
	void GetDelivered();

	UPROPERTY()
	float Price = 20;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteractionTriggered_Implementation(ACarpenterCharacter* Character) override;
};
