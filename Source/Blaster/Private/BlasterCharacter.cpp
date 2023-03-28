// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlasterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerFireComponent.h"


//////////////////////////////////////////////////////////////////////////
// ABlasterCharacter

ABlasterCharacter::ABlasterCharacter()
{
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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//ADSCamLoc = CreateDefaultSubobject<USceneComponent>(TEXT("ADSCamLocSceneComp"));
	//ADSCamLoc->SetupAttachment(GetMesh(), TEXT("gun_r"));

	IronSight_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("IronSight"));
	IronSight_Scene->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FPS_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("FPS"));
	FPS_Scene->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	FireComponent = CreateDefaultSubobject<UPlayerFireComponent>(TEXT("FireComp"));

	//timelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("timelineComponent"));
}

void ABlasterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("play"), true);
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	FollowCamVector = FollowCamera->GetRelativeLocation();
	/*TimelineFinishedEvent.BindUFunction(this, FName("ASDTimelineFinish"));
	timelineComp->SetTimelineFinishedFunc(TimelineFinishedEvent);
	ADSReturnFunction.BindUFunction(this, FName("ADSCamMove"));
	if(ADSFloat)
	{
		timelineComp->AddInterpFloat(ADSFloat, ADSReturnFunction);
	}
	timelineComp->SetLooping(false);*/
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABlasterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		SetUpInputDelegate.Broadcast(EnhancedInputComponent);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("BroadCast"), true);
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Look);

	}

}

void ABlasterCharacter::Move(const FInputActionValue& Value)
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

void ABlasterCharacter::Look(const FInputActionValue& Value)
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

//타임라인이 끝나면 실행되는 함수
//void ABlasterCharacter::ADSTimelineFinish()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("ADS timeline finish"), true);
//}
////타임라인을 실행시키는 함수
//void ABlasterCharacter::ADSTimelineStart()
//{
//	timelineComp->PlayFromStart();
//}
//타임라인중에 반환값을 받는 함수
//void ABlasterCharacter::ADSCamMove(float alpha)
//{
//	//카메라를 원래 위치에서 조준 위치로 옮긴다
//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("timeline"), true);
//	//ADSCamera->SetRelativeLocation(FMath::Lerp(ADSCamLoc->GetRelativeLocation(), ));
//}



