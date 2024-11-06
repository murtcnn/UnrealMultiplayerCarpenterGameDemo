// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Carpenter/Interfaces/Interaction_Interface.h"
#include "GameFramework/Actor.h"
#include "ColorTable.generated.h"

class AMyGameState;
class AFurniture;
class USphereComponent;

UCLASS()
class CARPENTER_API AColorTable : public AActor, public IInteraction_Interface
{
	GENERATED_BODY()

	// Collision Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	// Static Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	
public:	
	// Sets default values for this actor's properties
	AColorTable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterial* TableColor;
	
	UFUNCTION()
	bool GetHasFurniture() const;

	UFUNCTION()
	void SetHasFurniture(bool bNewState);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bHasFurniture = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	AFurniture* PlacedFurniture;

	UPROPERTY()
	AMyGameState* MyGameState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
						 AActor* OtherActor,
						 UPrimitiveComponent* OtherComp,
						 int32 OtherBodyIndex,
						 bool bFromSweep,
						 const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnInteractionTriggered_Implementation(ACarpenterCharacter* Character) override;
};
