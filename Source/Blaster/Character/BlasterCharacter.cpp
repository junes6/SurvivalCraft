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
	// 스프링암 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	// 전투 컴포넌트 설정
	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	// 전투 컴포넌트가 모든 클라이언트에 복제될 수 있게 설정한다.
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

	// 복제할 무기를 등록
	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);

}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Combat 컴포넌트가 유효한 경우
	if (Combat)
	{
		// 컴포넌트 내부에 가지고 있는 캐릭터에 접근해서 해당 클래스 데이터를 전달해준다. (frined Class를 통해 접근 가능)
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
	//조준중일때 상하 움직임
	/*if (FireComponent->bADS)
	{
		ADSGun->AddLocalRotation(FRotator(0, 0, -Value));
	}*/
}

void ABlasterCharacter::EquipButtonPressed()
{
	if (Combat)
	{	
		// 서버이면
		if (HasAuthority())
		{
			// 전투 컴포넌트의 장착무기는 오버랩된 무기를 전달
			Combat->EquipWeapon(OverlappingWeapon);
			
		}
		// 실패할 경우 무기장착 함수를 다시 호출한다
		else
		{
			// 서버가 아닌경우 RPC 호출
			ServerEquipButtonPressed();

		}
	}
}

// 구현부에는 _Implementation을 붙여준다.
void ABlasterCharacter::ServerEquipButtonPressed_Implementation()
{
	// RPC는 오직 서버에서만 실행된다.
	if (Combat)
	{
		// 전투 컴포넌트의 장착무기는 오버랩된 무기를 전달
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

	//델리게이트 브로드케스트
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
		// bIsCrouched = true로 변경한다.
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
	// 전투컴포넌트를 연결시켜준다.
	return (Combat && Combat->bAiming);
}


