// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrderPlacementArea.generated.h"

class AFurniture;
class AMyPlayerController;
class AMyHUD;
class AMyGameState;
class USphereComponent;

UCLASS()
class CARPENTER_API AOrderPlacementArea : public AActor
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
	AOrderPlacementArea();

	UPROPERTY()
	AMyGameState* MyGameState;

	UPROPERTY()
	AMyHUD* MyHUD;

	UPROPERTY()
	AMyPlayerController* MyPlayerController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	AFurniture* LatestPlacedFurniture;

	UFUNCTION()
	bool CheckFurnitureAndOrder(AFurniture* Furniture);

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
};
