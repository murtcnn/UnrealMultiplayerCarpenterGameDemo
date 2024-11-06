// Fill out your copyright notice in the Description page of Project Settings.


#include "Carpenter/Interactables/OrderPlacementArea.h"

#include "Carpenter/Interactables/Furniture.h"
#include "Carpenter/GameMode/MyGameState.h"
#include "Carpenter/GameMode/MyHUD.h"
#include "Carpenter//GameMode/MyPlayerController.h"
#include "Components/SphereComponent.h"

// Sets default values
AOrderPlacementArea::AOrderPlacementArea()
{
	bReplicates = true;
	NetUpdateFrequency = 5;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	// Sphere Collision
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(200);
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AOrderPlacementArea::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AOrderPlacementArea::OnSphereEndOverlap);
}

// Called when the game starts or when spawned
void AOrderPlacementArea::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
	MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD());
}

bool AOrderPlacementArea::CheckFurnitureAndOrder(AFurniture* Furniture)
{
	UStaticMesh* CurrentOrderMesh;
	UMaterialInterface* CurrentOrderMaterial;
	MyGameState->GetCurrentOrder(CurrentOrderMesh, CurrentOrderMaterial);
	if (Furniture->GetFurnitureMaterial() == CurrentOrderMaterial && Furniture->GetFurnitureMesh() == CurrentOrderMesh)
	{
		return true;
	}

	return false;
}

// Called every frame
void AOrderPlacementArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrderPlacementArea::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LatestPlacedFurniture = Cast<AFurniture>(OtherActor);
	if (LatestPlacedFurniture)
	{
		MyGameState->CheckPlacedOrder(LatestPlacedFurniture);
	}
}

void AOrderPlacementArea::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

