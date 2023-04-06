// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/WidgetComponent.h>
#include "Net/UnrealNetwork.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/BlasterComponents/CombatComponent.h"
#include <GameFramework/CharacterMovementComponent.h>

#include "BlasterAnimInstance.h"
#include "BlasterGameInstance.h"
#include "BlasterPlayerController.h"
#include "BlasterPlayerState.h"
#include "MySpectatorPawn.h"
#include "Blaster/BlasterComponents/PlayerFireComponent.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/HUD/PlayerUIWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

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

	DOREPLIFETIME(ABlasterCharacter, curHP);
	DOREPLIFETIME(ABlasterCharacter, ammo);
	DOREPLIFETIME(ABlasterCharacter, GunFire);
	DOREPLIFETIME(ABlasterCharacter, color);
	DOREPLIFETIME(ABlasterCharacter, team);

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

	animInstance = Cast<UBlasterAnimInstance>(GetMesh()->GetAnimInstance());

	pc = Cast<ABlasterPlayerController>(GetController());

	ps = Cast<ABlasterPlayerState>(GetPlayerState());
	//서버에서 hp를 변경하고 복제된다
	if(HasAuthority())
	{
		curHP = maxHP;
	}

	//서버에서 부활하면 실행되지 않는 이유
	if(pc&&pc->IsLocalController())
	{
		FString isServer = HasAuthority() ? FString("server") : FString("client");
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("UIIUIUIUIUIU : %s"), *isServer));
		UI = pc->playerUIWidget;
	}
	if(ps)
	{
		ps->SetTeamDele.AddUObject(this, &ABlasterCharacter::ServerSetTeam);
	}

	//색상변경 준비
	UMaterialInterface* base_mat = GetMesh()->GetMaterial(0);
	if (base_mat)
	{
		mat = UMaterialInstanceDynamic::Create(base_mat, this);
		GetMesh()->SetMaterial(0, mat);
	}
}

void ABlasterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!pc)
	{
		pc = Cast<ABlasterPlayerController>(GetController());
	}
	if(!ps)
	{
		ps = Cast<ABlasterPlayerState>(GetPlayerState());
		if(ps)
		{
			ps->SetTeamDele.AddUObject(this, &ABlasterCharacter::ServerSetTeam);
		}
	}

	if(mat && !bSetColor)
	{
		ServerSetTeamColor();
		bSetColor = true;
	}
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

void ABlasterCharacter::OnRep_SetHP_Persent()
{
	/*if(GetController()->IsLocalController())
	{
		ABlasterPlayerController* pc = Cast<ABlasterPlayerController>(GetController());
		if(pc && pc->playerUIWidget)
		{
			pc->playerUIWidget->text_HP->SetText(FText::AsNumber(curHP));
			pc->playerUIWidget->pb_HP->SetPercent(curHP/maxHP);
		}
	}*/
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
	PlayerInputComponent->BindAction(TEXT("ViewInfo"), IE_Pressed, this, &ABlasterCharacter::ViewInfo);
	PlayerInputComponent->BindAction(TEXT("ViewInfo"), IE_Released, this, &ABlasterCharacter::HideInfo);


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

void ABlasterCharacter::DecreaseHP(int32 value, class ABlasterCharacter* dealer)
{
	curHP -= value;
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("%d"), curHP));

	if(curHP <= 0)
	{
		FString isServer = HasAuthority() ? FString("server") : FString("client");
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("curHP : %s"), *isServer));
		DieProcess();

		//델리게이트를 플레이어 스테이트에 브로드케스트한다
		//Cast<UBlasterGameInstance>(GetGameInstance())->KillDelegate.Broadcast(dealer);
		ABlasterPlayerState* enemyps = Cast<ABlasterPlayerState>(dealer->GetPlayerState());
		enemyps->SetKillCount(1);
		if (ps)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red, FString::Printf(TEXT("SetDeathCount")));
			ps->SetDeathCount(1);
		}
	}
}

void ABlasterCharacter::DieProcess()
{
	MultiPlayDie();
}

void ABlasterCharacter::ChangeSpectatorMode()
{
	ABlasterGameMode* gm = Cast<ABlasterGameMode>(GetWorld()->GetAuthGameMode());
	if(gm)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		blasterSpectator = GetWorld()->SpawnActor<AMySpectatorPawn>(TSub_spectatorPawn, GetActorLocation(), GetActorRotation(), param);
		if (blasterSpectator != nullptr)
		{
			GetController()->Possess(blasterSpectator);
		}

		FTimerHandle TimerHandle_Respawn;
		GetWorldTimerManager().SetTimer(TimerHandle_Respawn, FTimerDelegate::CreateLambda([&]()
			{
			//BP에 spectator설정
				blasterSpectator->GetController()->Possess(this);
				blasterSpectator->Destroy();
			//pc가 캐스팅되지 않으면 다시 캐스팅한다
				Respawn:
				if(pc)
				{
					pc->Respawn(this);
				}
				else
				{
					pc = Cast<ABlasterPlayerController>(GetController());
					goto Respawn;
				}
			}), respawnTime, false);
	}
}

void ABlasterCharacter::SetWidget()
{
	FTimerHandle TimerHandle_UI;
	GetWorldTimerManager().SetTimer(TimerHandle_UI, FTimerDelegate::CreateLambda([&]()
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("CombatUI")));
			UI->SetCombatWidget();
		}), respawnTime, false);
}

void ABlasterCharacter::MultiPlayDie_Implementation()
{
	//로컬에서 플레이어 스테이트를 연다

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("play die"));
	animInstance->bDie = true;
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bUseControllerRotationYaw = false;
	FTimerHandle TimerHandle_Death;
	GetWorldTimerManager().SetTimer(TimerHandle_Death, FTimerDelegate::CreateLambda([&]()
		{
			//게임모드가 있는 서버에서 포제스를 한다
			if (HasAuthority())
			{
				ChangeSpectatorMode();
			}
			
			//위젯이 있는 로컬 플레이어에서 위젯을 설정한다
			//if (GetController() && GetController()->IsLocalController())
			//{
				if (UI)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("RespawnUI")));
					//위젯이 있는 로컬에서 데스카운트를 설정한다
					UI->SetRespawnWidget();
					SetWidget();
				}
			//}
		}), 1.5f, false);
}

void ABlasterCharacter::ServerOnDamage_Implementation(int32 value, ABlasterCharacter* dealer)
{
	DecreaseHP(value, dealer);
	FString isServer = HasAuthority() ? FString("server") : FString("client");
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("ServerOnDamagme : %s"), *isServer));
}

void ABlasterCharacter::ViewInfo()
{
	if(UI)
	{
		UI->ViewInfo(true);
	}
}

void ABlasterCharacter::HideInfo()
{
	if (UI)
	{
		UI->ViewInfo(false);
	}
}

void ABlasterCharacter::ServerSetTeamColor_Implementation()
{
	MultiSetTeamColor();
}

void ABlasterCharacter::MultiSetTeamColor_Implementation()
{
	//MultiSetTeamColor(value);
	FString isServer = HasAuthority() ? FString("server") : FString("client");
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("ServerCCCCCCCCCC : %s"), *isServer));
	if (team == 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("MUltiSetTeamColor"));
		//red
		mat->SetVectorParameterValue(FName("Color_Multiplier"), (FLinearColor)FVector(1, 0.125397, 0));
	}
	else if (team == 2)
	{
		//blue
		mat->SetVectorParameterValue(FName("Color_Multiplier"), (FLinearColor)FVector(0.108294, 0.218351, 1));
	}
}

void ABlasterCharacter::ServerSetTeam_Implementation(int32 value)
{
	if (!ps)
	{
		return;
	}
	team = ps->team;
}
