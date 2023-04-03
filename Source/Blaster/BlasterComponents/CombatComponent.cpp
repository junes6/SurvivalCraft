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

	// ���Ⱑ ����Ǹ� �ش� ��������� ��� Ŭ���̾�Ʈ���� �ݿ��ȴ�.
	DOREPLIFETIME(UCombatComponent, EquippedWeapon);
	DOREPLIFETIME(UCombatComponent, bAiming);
}

void UCombatComponent::EquipWeapon(class AWeapon* WeaponToEquip)
{
	// ������Ʈ���� ������ ĳ���Ͱ� ��ȿ�� ��� || ���޵� ���Ⱑ ��ȿ�� ���
	if (Character == nullptr || WeaponToEquip == nullptr)
	{
		// ���߿� �ϳ��� ��ȿ���� ������ ��ȯ
		return;
	}

	//��������Ʈ ��ε��ɽ�Ʈ PlayerFireComponent���� �޴´�
	ABlasterCharacter* player = Cast<ABlasterCharacter>(GetOwner());
	setUpWeapon.Broadcast(WeaponToEquip, player);

	// ���� �����ϱ�
	EquippedWeapon = WeaponToEquip;
	
	// Weapon.h���� ������ �ִ� SetWeaponState�� �������·� ����
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);

	// ĳ������ �޽ÿ��� ������ HandSocket�� �����Ѵ�.
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));

	// HandSocket�� ��ȿ�� ���
	if (HandSocket)
	{
		// HandSocket�� �����ȹ��⸦, ĳ���� ���̷�Ż�޽ÿ� ����
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}

	// ������ ����� �ش� Character�� �����ڷ� ����
	EquippedWeapon->SetOwner(Character);

	
}

