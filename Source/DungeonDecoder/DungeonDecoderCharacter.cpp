// Copyright Epic Games, Inc. All Rights Reserved.

#include "DungeonDecoderCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DungeonDecoder.h"
#include "CollectableItem.h"
#include "Lock.h"


ADungeonDecoderCharacter::ADungeonDecoderCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	TraceOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("TraceOrigin"));
	TraceOrigin->SetupAttachment(FollowCamera); // Attach directly to camera
	TraceOrigin->SetRelativeLocation(FVector(0.f, 0.f, 0.f)); // adjust offset if needed

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ADungeonDecoderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeonDecoderCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeonDecoderCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeonDecoderCharacter::Look);

		
		EnhancedInputComponent->BindAction(IA_Interact, ETriggerEvent::Started, this, &ADungeonDecoderCharacter::OnInteract);

	}
	else
	{
		UE_LOG(LogDungeonDecoder, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADungeonDecoderCharacter::OnInteract(const FInputActionValue& Value)
{
	// This function can be expanded to handle interaction logic
	FVector Start = TraceOrigin->GetComponentLocation();

	FVector End = Start + (TraceOrigin->GetForwardVector() * TraceDistance);
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);
	FCollisionShape InteractionSphere = FCollisionShape::MakeSphere(InteractionRadius);

	FHitResult HitResult;
	bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, InteractionSphere);
	if (bHasHit) {
		AActor* HitActor = HitResult.GetActor();

		if ( HitActor->ActorHasTag("CollectableItem")) {
			UE_LOG(LogDungeonDecoder, Log, TEXT("Interacted with collectable : %s"), *HitActor->GetName());

			ACollectableItem* CollectableItem= Cast<ACollectableItem>(HitActor);
			if (CollectableItem) {
				ItemList.Add(CollectableItem->ItemName);
				CollectableItem->Destroy();
			}

					
			// Further interaction logic can be implemented here
		}else if(HitActor->ActorHasTag("Lock")) {
			UE_LOG(LogDungeonDecoder, Log, TEXT("Interacted with collectable Lock: %s"), *HitActor->GetName());
			ALock* LockItem= Cast<ALock>(HitActor);
			if (LockItem) {
				if (!LockItem->GetIsKeyPlaced()) {
				int32 IteamsRemoved = ItemList.RemoveSingle(LockItem->KeyName);
				if (IteamsRemoved) {
					LockItem->SetIsKeyPlaced(true);
				}
				else
				{
					UE_LOG(LogDungeonDecoder, Log, TEXT("Key item %s not found in inventory."), *LockItem->KeyName);
				}
				}
				else {
					if (LockItem->GetIsKeyPlaced()) {
						ItemList.Add(LockItem->KeyName);
						LockItem->SetIsKeyPlaced(false);

					}

				}
			}
		}
	}
	else {
		UE_LOG(LogDungeonDecoder, Log, TEXT("No actor hit during interaction."));
	
	}
}




void ADungeonDecoderCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ADungeonDecoderCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ADungeonDecoderCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ADungeonDecoderCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeonDecoderCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ADungeonDecoderCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
