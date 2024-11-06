// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interaction_Interface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/TextFilterExpressionEvaluator.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACarpenterCharacter

ACarpenterCharacter::ACarpenterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ACarpenterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ACarpenterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	InteractionTrace();
}

void ACarpenterCharacter::InteractionTrace()
{
	UWorld* World = GetWorld();
	if(!World) return;

	FVector StartPoint = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector EndPoint = StartPoint + ForwardVector * 5000;

	// Set up collision query params
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);  // Ignore self

	// The hit result will store information about the trace
	FHitResult OutHit;
	
	bool bHit = World->LineTraceSingleByChannel(OutHit, StartPoint, EndPoint, ECC_Visibility, CollisionParams);

	// Optional: Draw a debug line to visualize the trace
	DrawDebugLine(World, StartPoint, EndPoint, FColor::Green, false, 0, 0, 1);
	
	if (bHit && OutHit.GetActor())
	{
		TraceHitActor = OutHit.GetActor();
	}
	else
	{
		TraceHitActor = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACarpenterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACarpenterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACarpenterCharacter::Look);

		// Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ACarpenterCharacter::Interact);

		// Toggling UI
		EnhancedInputComponent->BindAction(ToggleUIAction, ETriggerEvent::Started, this, &ACarpenterCharacter::ToggleUI);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACarpenterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACarpenterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACarpenterCharacter::Interact(const FInputActionValue& Value)
{
	if (!HasAuthority())
	{
		Server_Interact(Value);
		return;
	}

	// If the holding actor has the interface, send the interaction event.
	if (HoldingActor && HoldingActor->GetClass()->ImplementsInterface(UInteraction_Interface::StaticClass()))
	{
		IInteraction_Interface::Execute_OnInteractionTriggered(HoldingActor, this);
		HoldingActor = nullptr;
		return;
	}
	
	// If the trace hit actor has the interface, send the interaction event.
	if (TraceHitActor && TraceHitActor->GetClass()->ImplementsInterface(UInteraction_Interface::StaticClass()))
	{
		IInteraction_Interface::Execute_OnInteractionTriggered(TraceHitActor, this);

		// Only if the interacted trace hit actor is a holdable object, set it as the holding actor.
		if (TraceHitActor->ActorHasTag(TEXT("Holdable"))) HoldingActor = TraceHitActor;
	}
}

void ACarpenterCharacter::Server_Interact_Implementation(const FInputActionValue& Value)
{
	Interact(Value);
}

void ACarpenterCharacter::ToggleUI(const FInputActionValue& Value)
{
	
}
