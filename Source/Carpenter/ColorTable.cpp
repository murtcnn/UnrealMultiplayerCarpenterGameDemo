// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorTable.h"

#include "Furniture.h"
#include "MyGameState.h"
#include "Components/SphereComponent.h"

// Sets default values
AColorTable::AColorTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	// Sphere Collision
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->InitSphereRadius(200);
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AColorTable::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AColorTable::OnSphereEndOverlap);

}

bool AColorTable::GetHasFurniture() const
{
	return bHasFurniture;
}

void AColorTable::SetHasFurniture(bool bNewState)
{
	bHasFurniture = bNewState;
}

// Called when the game starts or when spawned
void AColorTable::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
	
}

// Called every frame
void AColorTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AColorTable::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!GetHasFurniture() && Cast<AFurniture>(OtherActor))
	{
		SetHasFurniture(true);
		PlacedFurniture = Cast<AFurniture>(OtherActor);
	}
}

void AColorTable::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetHasFurniture() && OtherActor == PlacedFurniture)
	{
		SetHasFurniture(false);
	}
}

void AColorTable::OnInteractionTriggered_Implementation(ACarpenterCharacter* Character)
{
	if (GetHasFurniture() && PlacedFurniture && TableColor && PlacedFurniture->GetFurnitureMaterial() != TableColor)
	{
		bool bHasEnoughMoney = false;
		MyGameState->SpendMoney(1, bHasEnoughMoney);
		if (bHasEnoughMoney)
		{
			PlacedFurniture->SetFurnitureMaterial(TableColor);
		}
	}
}

