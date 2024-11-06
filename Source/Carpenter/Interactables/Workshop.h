// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Furniture.h"
#include "Carpenter/GameMode/MyPlayerController.h"
#include "GameFramework/Actor.h"
#include "Carpenter/Interfaces/Workshop_Interface.h"
#include "Components/SphereComponent.h"
#include "Workshop.generated.h"

class ACarpenterCharacter;
class AMyHUD;

UCLASS()
class CARPENTER_API AWorkshop : public AActor, public IWorkshop_Interface
{
	GENERATED_BODY()

	// Collision Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionSphere;

	// Static Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	// Spawn Point
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnPoint;
	
public:	
	// Sets default values for this actor's properties
	AWorkshop();
	
	UPROPERTY()
	AMyGameState* MyGameState;

	UPROPERTY()
	AMyHUD* MyHUD;

	UPROPERTY()
	AMyPlayerController* MyPlayerController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AFurniture> BP_Furniture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* FurnitureMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	// Overlapping
	UPROPERTY()
	TArray<ACarpenterCharacter*> OverlappedCharacters;

	
	// Money
	UFUNCTION()
	void UpdateShouldCreate(float FurnitureCost);
	
	// Furniture
	UPROPERTY()
	bool bShouldCreate = false;

	UPROPERTY()
	bool bIsBeingUsed = false;
	
	UFUNCTION()
	void ContinueFurnitureCreationProgress(float InDeltaTime);

	UPROPERTY()
	float CurrentCreationProgress = 0;

	UFUNCTION(NetMulticast, Reliable)
	void UpdateFurnitureCreationProgress(float Progress);
	
	UFUNCTION(BlueprintCallable)
	void CreateFurniture();

	virtual void Auth_CreateFurniture_Implementation() override;


};
