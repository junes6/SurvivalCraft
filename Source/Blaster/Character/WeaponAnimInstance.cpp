// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"

#include "BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void UWeaponAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	//player = Cast<ABlasterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	player = Cast<ABlasterCharacter>(GetOwningActor());
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!player){ return; }

	FRotator viewRot = player->GetBaseAimRotation();
	FRotator playerRot = player->GetActorRotation();
	FRotator deltaRot = playerRot - viewRot;
	weaponPitch = FMath::Clamp(deltaRot.GetNormalized().Pitch, -45.f, 45.f);
}

void UWeaponAnimInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//weaponPitch 값이 동기화되어서 총알의 스폰 위치를 일치하게한다
	DOREPLIFETIME(UWeaponAnimInstance, weaponPitch);
}