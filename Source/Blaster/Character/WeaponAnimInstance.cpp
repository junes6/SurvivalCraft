// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAnimInstance.h"

#include "BlasterCharacter.h"
#include "Kismet/GameplayStatics.h"

void UWeaponAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	player = Cast<ABlasterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UWeaponAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!player){return;}
	FRotator viewRot = player->GetBaseAimRotation();
	FRotator playerRot = player->GetActorRotation();
	FRotator deltaRot = playerRot - viewRot;
	weaponPitch = FMath::Clamp(deltaRot.GetNormalized().Pitch, -45.f, 45.f);
}
