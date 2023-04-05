// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "Blaster/BlasterComponents/GunFireComponent.h"
#include "Blaster/BlasterComponents/PlayerFireComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// BlasterCharacter�� ĳ�����ؼ� ������ ��Ƶд�.
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// BlasterCharacter�� ĳ���õ��� ������ �ٽ� ĳ���� �õ��Ѵ�.
	if (!BlasterCharacter)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}

	if (!BlasterCharacter)
	{
		return;
	}

	// Velocity ������ BlasterCharacter �ӵ��� ��´�.
	FVector Velocity = BlasterCharacter->GetVelocity();
	// Velocity�� Z���� 0���� ���� �����Ѵ�.
	Velocity.Z = 0.f;
	// Speed ������ Velocity ũ�⸦ ��´�.
	Speed = Velocity.Size();
	// BlasterCharacter�� IsFalling �Լ����� ��ȯ�Ǵ� bool���� ��´�.
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

	// ĳ������ bool ������ bIsCrouched�� �޾ƿͼ� ����
	bIsCrouched = BlasterCharacter->bIsCrouched;

	bAiming = BlasterCharacter->IsAiming();

	FRotator viewRot = BlasterCharacter->GetBaseAimRotation();
	FRotator playerRot = BlasterCharacter->GetActorRotation();
	FRotator deltaRot = playerRot - viewRot;
	playerAimPitch = FMath::Clamp(deltaRot.GetNormalized().Pitch, -45.f, 45.f);

	
}

void UBlasterAnimInstance::AnimNotify_ReloadingEnd()
{
	//���� ��Ʈ�ѷ��� ������ RPC�� ������ ��û�Ѵ�
	if(BlasterCharacter->GetController() && BlasterCharacter->GetController()->IsLocalController())
	{
		BlasterCharacter->GunFire->ServerReload();
	}
}

//void UBlasterAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
	//Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//weaponPitch ���� ����ȭ�Ǿ �Ѿ��� ���� ��ġ�� ��ġ�ϰ��Ѵ�
	//DOREPLIFETIME(UBlasterAnimInstance, playerAimPitch);
//}