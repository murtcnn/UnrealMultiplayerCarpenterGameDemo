// Fill out your copyright notice in the Description page of Project Settings.


#include "Workshop.h"

#include "Carpenter/CarpenterCharacter.h"
#include "Furniture.h"
#include "Carpenter/GameMode/MyGameState.h"
#include "Carpenter/GameMode/MyHUD.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWorkshop::AWorkshop()
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
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWorkshop::OnSphereOverlap);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AWorkshop::OnSphereEndOverlap);

	// Spawn Point
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWorkshop::BeginPlay()
{
	Super::BeginPlay();

	MyGameState = Cast<AMyGameState>(GetWorld()->GetGameState());
	MyPlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD());
}

void AWorkshop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void AWorkshop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldCreate)
	{
		ContinueFurnitureCreationProgress(DeltaTime);
	}
}

void AWorkshop::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                const FHitResult& SweepResult)
{
	// Check if the overlapped actor is a character.
	if (ACarpenterCharacter* Character = Cast<ACarpenterCharacter>(OtherActor))
	{
		// Add the overlapped to the array.
		OverlappedCharacters.AddUnique(Character);

		// If this is not the first player to enter the bounds, do nothing.
		if (OverlappedCharacters.Num() != 1)
		{
			return;
		}
		
		// Turn on the workshop widget only on the local player.
		if (MyHUD && Character->IsLocallyControlled())
		{
			MyHUD->TurnOnWorkshopWidget(this);
		}

		UpdateShouldCreate(10);
	}
}

void AWorkshop::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the overlapped actor is a character.
	if (ACarpenterCharacter* Character = Cast<ACarpenterCharacter>(OtherActor))
	{
		// Remove the character that left the bounds from the array.
		OverlappedCharacters.RemoveSingle(Character);

		// If the player that just left the bounds wasn't the last player ın the bounds, do nothing.
		if (OverlappedCharacters.Num() != 0)
		{
			return;
		}
		
		// Turn off the workshop widget.
		if (MyHUD && Character->IsLocallyControlled())
		{
			MyHUD->TurnOffWorkshopWidget();
		}

		bShouldCreate = false;
		UpdateFurnitureCreationProgress(CurrentCreationProgress);
	}
}

void AWorkshop::UpdateShouldCreate(float FurnitureCost)
{
	if (!MyGameState)
	{
		return;
	}

	if (MyGameState->GetCurrentMoney() < FurnitureCost)
	{
		return;
	}

	bShouldCreate = true;
}

void AWorkshop::ContinueFurnitureCreationProgress(float InDeltaTime)
{
	if (CurrentCreationProgress >= 1)
	{
		CreateFurniture();
		CurrentCreationProgress = 0;
		return;
	}

	// Update the progress.
	CurrentCreationProgress += 0.1 * InDeltaTime;

	// Update the UI.
	if (MyHUD && MyHUD->WBP_WorkshopUI)
	{
		MyHUD->WBP_WorkshopUI->UpdateCreationProgressBar(CurrentCreationProgress);
	}
}

void AWorkshop::UpdateFurnitureCreationProgress_Implementation(float Progress)
{
	CurrentCreationProgress = Progress;

	// Update the UI.
	if (MyHUD && MyHUD->WBP_WorkshopUI)
	{
		MyHUD->WBP_WorkshopUI->UpdateCreationProgressBar(CurrentCreationProgress);
	}
}

// Create Furniture
void AWorkshop::CreateFurniture()
{
	if (HasAuthority())
	{
		bool bHasEnoughMoney = false;
		MyGameState->SpendMoney(10, bHasEnoughMoney);
		if (bHasEnoughMoney && FurnitureMesh)
		{
			AFurniture* SpawnedFurniture = GetWorld()->SpawnActor<AFurniture>(BP_Furniture, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());
			SpawnedFurniture->SetFurnitureMesh(FurnitureMesh);
		}
	}
}

void AWorkshop::Auth_CreateFurniture_Implementation() // From PlayerController.
{
	CreateFurniture();
}
