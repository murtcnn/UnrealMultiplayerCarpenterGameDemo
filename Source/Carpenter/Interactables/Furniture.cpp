// Fill out your copyright notice in the Description page of Project Settings.

#include "Carpenter/CarpenterCharacter.h"
#include "Furniture.h"

// Sets default values
AFurniture::AFurniture()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	Tags.Add(TEXT("Holdable"));

	// Static Mesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

void AFurniture::SetHasOwner(bool HasOwner)
{
	bHasOwner = HasOwner;
}

UMaterial* AFurniture::GetFurnitureMaterial() const
{
	return Cast<UMaterial>(StaticMesh->GetMaterial(0));
}

void AFurniture::SetFurnitureMaterial_Implementation(UMaterial* Material)
{
	if (StaticMesh && Material)
	{
		StaticMesh->SetMaterial(0, Material);
	}
}

UStaticMesh* AFurniture::GetFurnitureMesh() const
{
	return StaticMesh->GetStaticMesh();
}

void AFurniture::SetFurnitureMesh_Implementation(UStaticMesh* FurnitureMesh)
{
	if (StaticMesh && FurnitureMesh)
	{
		StaticMesh->SetStaticMesh(FurnitureMesh);
	}
}

void AFurniture::GetDelivered()
{
	Destroy();
}

// Called when the game starts or when spawned
void AFurniture::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFurniture::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFurniture::OnInteractionTriggered_Implementation(ACarpenterCharacter* Character)
{
	if (!bHasOwner)
	{
		AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("CarryFurniture"));
		SetHasOwner(true);
		return;
	}

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetHasOwner(false);
}


