// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;

	ServerSetAiming(bIsAiming);
}

void UCombatComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
}


void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 무기가 변경되면 해당 변경사항이 모든 클라이언트에게 반영된다.
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
}

void UCombatComponent::EquipWeapon(class AWeapon* WeaponToEquip)
{
	// 컴포넌트에서 선언한 캐릭터가 유효한 경우 || 전달된 무기가 유효한 경우
	if (Character == nullptr || WeaponToEquip == nullptr)
	{
		// 둘중에 하나라도 유효하지 않으면 반환
		return;
	}

	//델리게이트 브로드케스트 PlayerFireComponent에서 받는다
	ABlasterCharacter* player = Cast<ABlasterCharacter>(GetOwner());
	setUpWeapon.Broadcast(WeaponToEquip, player);

	// 무기 부착하기
	EquippedWeapon = WeaponToEquip;
	
	// Weapon.h에서 가지고 있는 SetWeaponState를 장착상태로 변경
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	// 캐릭터의 메시에서 소켓을 HandSocket에 저장한다.
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	// HandSocket이 유효한 경우
	if (HandSocket)
	{
		// HandSocket에 장착된무기를, 캐릭터 스켈레탈메시에 부착
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}

	// 장착된 무기는 해당 Character를 소유자로 설정
	EquippedWeapon->SetOwner(Character);

	
}

