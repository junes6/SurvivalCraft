// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "EngineUtils.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Blaster/BlasterComponents/GunFireComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	// ���� ������Ʈ ����
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// �ݸ��� ä�� Block
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// ������ �浹�� ����
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Sphere ������Ʈ ����
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	// Sphere �浹 ����(Ŭ���̾�Ʈ������ �浹ó������ �ʰ� �������� ����)
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �Ⱦ� ���� �߰�
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	GunFire = CreateDefaultSubobject<UGunFireComponent>(TEXT("GunFire"));
	GunFire->SetIsReplicated(true);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	// GetLocalRole() == ENetRole::ROLE_Authority)
	if (HasAuthority())
	{
		// �������� �浹ó��
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// ������ �̺�Ʈ ���ε�
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);

		// ������ ���� �̺�Ʈ ���ε�
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}

	// �����ϸ� PickupWidget �Ⱥ��̰� ����
	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// �����ϰ� ���� ������ ����� �� �ִ�.
	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ �̺�Ʈ �߻��� BlasterCharacter�� ����ȯ
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		// PickupWidget->SetVisibility(true);

		// �������̺�Ʈ�� �߻��ϸ� BlasterCharacter�� SetOverlappingWeapon()�Լ��� ȣ���Ѵ�.
		BlasterCharacter->SetOverlappingWeapon(this);
	}

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// ������ �̺�Ʈ �߻��� BlasterCharacter�� ����ȯ
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		// PickupWidget->SetVisibility(true);

		BlasterCharacter->SetOverlappingWeapon(nullptr);
	}
}

void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;

	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:

		// ���Ⱑ ������ ������ �����.
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if(PickupWidget)
	{
		// PickupWidget Visibility ����
		PickupWidget->SetVisibility(bShowWidget);
	}
}

void AWeapon::GunFireStart()
{
	if (!GunFire) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("GunFire Null"), true);
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("call fire start"), true);
	GunFire->GunFireStart();
}

void AWeapon::GunFireStop()
{
	if (!GunFire) { return; }
	GunFire->GunFireStop();
}

//void AWeapon::AttachGun()
//{
//	ABlasterCharacter* player = nullptr;
//
//	float maxDistance = 200;
//	for (TActorIterator<ABlasterCharacter> it(GetWorld()); it; ++it)
//	{
//		float distance = GetDistanceTo(*it);
//
//		if (distance < maxDistance)
//		{
//			maxDistance = distance;
//			player = *it;
//		}
//	}
//
//	//�÷��̾ ���� �ƴϰ� ������Ʈ�� ���� �ƴ϶��
//	if (player && player->FireComponent)
//	{
//		player->FireComponent->GetAK(this);
//		USkeletalMeshComponent* playerMesh = player->GetMesh();
//		this->SetActorLocationAndRotation(playerMesh->GetSocketLocation(FName("gun_r")), playerMesh->GetSocketRotation(FName("gun_r")));
//		meshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("gun_r"));
//		isAttached = true;
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("FireComp Null"), true);
//	}
//}

