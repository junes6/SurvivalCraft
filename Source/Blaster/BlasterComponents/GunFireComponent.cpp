// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFireComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "PlayerFireComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Gun_Bullet.h"
#include "Blaster/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UGunFireComponent::UGunFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunFireComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	me = Cast<AWeapon>(GetOwner());
	gunMesh = Cast<USkeletalMeshComponent>(me->WeaponMesh);
	//UGameplayStatics::GetPlayerPawn
	localPlayerCamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	player = Cast<ABlasterCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}


// Called every frame
void UGunFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunFireComponent::GunFireStart()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutoFire, this, &UGunFireComponent::GunFire, 0.07, true, 0);
}

void UGunFireComponent::GunFire()
{
	FVector spawnLoc;
	if(player->FireComponent->bADS)
	{
		spawnLoc = player->ADSGun->GetSocketLocation(FName("Muzzle"));
	}
	else
	{
		spawnLoc = gunMesh->GetSocketLocation(FName("Muzzle"));
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("fire"), true);
	GetWorld()->SpawnActor<AGun_Bullet>(bullet, spawnLoc, player->ADSGun->GetSocketRotation("Muzzle"));

	if(fireAnim)
	{
		player->PlayAnimMontage(fireAnim);
	}
	
	//SpawnMuzzleEffect();
	//카메라 쉐이크
	//if(fireShakeIns->)
	if(fireShake)
	{
		if(fireShakeIns&&!fireShakeIns->IsFinished())
		{
			fireShakeIns->StopShake(true);
		}
		fireShakeIns = localPlayerCamManager->StartCameraShake(fireShake);
	}

	//반동
	Recoil();
}

void UGunFireComponent::GunFireStop()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("endFire"), true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutoFire);
	recoilCount = 0;
}

void UGunFireComponent::SpawnMuzzleEffect()
{
	if (me->muzzleEffect) {
		// This spawns the chosen effect on the owning WeaponMuzzle SceneComponent
		UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(me->muzzleEffect, me->meshComp, FName("MuzzleFlash"), FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	}
	//NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);
}

void UGunFireComponent::Recoil()
{
	if (recoilCount < 7) {
		player->AddControllerPitchInput(-0.5f);
		recoilCount += 1;

	}
	else if (recoilCount <= 16) {
		player->AddControllerYawInput(-1);
		if (recoilCount % 2 == 0) {
			player->AddControllerPitchInput(-0.6f);
		}
		else if (recoilCount % 2 == 1) {
			player->AddControllerPitchInput(0.3f);
		}

		recoilCount += 1;
	}
	else if (recoilCount <= 25) {
		player->AddControllerYawInput(1);

		recoilCount += 1;
	}
}

