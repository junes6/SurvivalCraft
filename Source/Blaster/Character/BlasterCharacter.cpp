// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/WidgetComponent.h>
#include "Net/UnrealNetwork.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/BlasterComponents/CombatComponent.h"
#include <GameFramework/CharacterMovementComponent.h>

#include "Blaster/BlasterComponents/PlayerFireComponent.h"

// Sets default values
ABlasterCharacter::ABlasterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FireComponent = CreateDefaultSubobject<UPlayerFireComponent>(TEXT("FireComp"));
	FireComponent->SetIsReplicated(true);
	// �������� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// ���� ������Ʈ ����
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	// ���� ������Ʈ�� ��� Ŭ���̾�Ʈ�� ������ �� �ְ� �����Ѵ�.
	Combat->SetIsReplicated(true);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;


	IronSight_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("IronSight"));
	IronSight_Scene->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FPS_Scene = CreateDefaultSubobject<USceneComponent>(TEXT("FPS"));
	FPS_Scene->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	ADSGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ADSGun"));
	ADSGun->SetupAttachment(GetMesh());
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ������ ���⸦ ���
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);

}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Combat ������Ʈ�� ��ȿ�� ���
	if (Combat)
	{
		// ������Ʈ ���ο� ������ �ִ� ĳ���Ϳ� �����ؼ� �ش� Ŭ���� �����͸� �������ش�. (frined Class�� ���� ���� ����)
		Combat->Character = this;
	}
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FollowCamVector = FollowCamera->GetRelativeLocation();
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABlasterCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABlasterCharacter::Turn(float Value)
{
	AddControllerYawInput(-Value);
}

void ABlasterCharacter::Lookup(float Value)
{
	AddControllerPitchInput(-Value);
	//�������϶� ���� ������
	/*if (FireComponent->bADS)
	{
		ADSGun->AddLocalRotation(FRotator(0, 0, -Value));
	}*/
}

void ABlasterCharacter::EquipButtonPressed()
{
	if (Combat)
	{	
		// �����̸�
		if (HasAuthority())
		{
			// ���� ������Ʈ�� ��������� �������� ���⸦ ����
			Combat->EquipWeapon(OverlappingWeapon);
			
		}
		// ������ ��� �������� �Լ��� �ٽ� ȣ���Ѵ�
		else
		{
			// ������ �ƴѰ�� RPC ȣ��
			ServerEquipButtonPressed();

		}
	}
}

// �����ο��� _Implementation�� �ٿ��ش�.
void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	// RPC�� ���� ���������� ����ȴ�.
	if (Combat)
	{
		// ���� ������Ʈ�� ��������� �������� ���⸦ ����
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}
}

void ABlasterCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

bool ABlasterCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}


void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//��������Ʈ ��ε��ɽ�Ʈ
	SetUpInputDelegate.Broadcast(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Equip"), IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABlasterCharacter::Lookup);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ABlasterCharacter::CrouchButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ABlasterCharacter::AimPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ABlasterCharacter::AimReleased);


}

void ABlasterCharacter::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		// bIsCrouched = true�� �����Ѵ�.
		Crouch();
	}
}

void ABlasterCharacter::AimPressed()
{
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void ABlasterCharacter::AimReleased()
{
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

bool ABlasterCharacter::IsAiming()
{
	/*if (Combat)
	{
		return Combat->bAiming;
	}*/
	// ����������Ʈ�� ��������ش�.
	return (Combat && Combat->bAiming);
}


