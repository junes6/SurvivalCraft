// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterAnimInstance.h"
#include "BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// BlasterCharacter를 캐스팅해서 정보를 담아둔다.
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// BlasterCharacter가 캐스팅되지 않으면 다시 캐스팅 시도한다.
	if (!BlasterCharacter)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}

	if (!BlasterCharacter)
	{
		return;
	}

	// Velocity 변수에 BlasterCharacter 속도를 담는다.
	FVector Velocity = BlasterCharacter->GetVelocity();
	// Velocity의 Z값은 0으로 임의 설정한다.
	Velocity.Z = 0.f;
	// Speed 변수에 Velocity 크기를 담는다.
	Speed = Velocity.Size();
	// BlasterCharacter의 IsFalling 함수에서 반환되는 bool값을 담는다.
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;

	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();

	// 캐릭터의 bool 변수인 bIsCrouched를 받아와서 연결
	bIsCrouched = BlasterCharacter->bIsCrouched;

	bAiming = BlasterCharacter->IsAiming();
}
