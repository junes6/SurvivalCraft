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

	// 무기 컴포넌트 생성
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// 콜리전 채널 Block
	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// 폰과의 충돌은 무시
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Sphere 컴포넌트 생성
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	// Sphere 충돌 설정(클라이언트에서는 충돌처리하지 않고 서버에서 수행)
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 픽업 위젯 추가
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
		// 서버에서 충돌처리
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		// 오버랩 이벤트 바인딩
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);

		// 오버랩 종료 이벤트 바인딩
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}

	// 시작하면 PickupWidget 안보이게 설정
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

	// 복제하고 싶은 변수를 등록할 수 있다.
	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 이벤트 발생시 BlasterCharacter로 형변환
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);

	if (BlasterCharacter)
	{
		// PickupWidget->SetVisibility(true);

		// 오버랩이벤트가 발생하면 BlasterCharacter에 SetOverlappingWeapon()함수를 호출한다.
		BlasterCharacter->SetOverlappingWeapon(this);
	}

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 오버랩 이벤트 발생시 BlasterCharacter로 형변환
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

		// 무기가 있으면 위젯을 숨긴다.
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void AWeapon::ShowPickupWidget(bool bShowWidget)
{
	if(PickupWidget)
	{
		// PickupWidget Visibility 설정
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
//	//플레이어가 널이 아니고 컴포넌트가 널이 아니라면
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

