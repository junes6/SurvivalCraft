// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_AK.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "BlasterCharacter.h"
#include "EngineUtils.h"
#include "GunFireComponent.h"
#include "PlayerFireComponent.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AGun_AK::AGun_AK()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);

	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if(TempMesh.Succeeded())
	{
		meshComp->SetSkeletalMesh(TempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));

	GunFire = CreateDefaultSubobject<UGunFireComponent>(TEXT("GunFire"));
}

// Called when the game starts or when spawned
void AGun_AK::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun_AK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!isAttached)
	{
		AttachGun();
	}
}

void AGun_AK::GunFireStart()
{
	if (!GunFire) { GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("GunFire Null"), true);
		return;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("call fire start"), true);
	GunFire->GunFireStart();
}

void AGun_AK::GunFireStop()
{
	if(!GunFire){ return; }
	GunFire->GunFireStop();
}

void AGun_AK::AttachGun()
{
	ABlasterCharacter* player = nullptr;

	float maxDistance = 200;
	for(TActorIterator<ABlasterCharacter> it(GetWorld()); it; ++it)
	{
		float distance = GetDistanceTo(*it);

		if(distance < maxDistance)
		{
			maxDistance = distance;
			player = *it;
		}
	}

	//플레이어가 널이 아니고 컴포넌트가 널이 아니라면
	if(player&&player->FireComponent)
	{
		player->FireComponent->GetAK(this);
		USkeletalMeshComponent* playerMesh = player->GetMesh();
		this->SetActorLocationAndRotation(playerMesh->GetSocketLocation(FName("gun_r")), playerMesh->GetSocketRotation(FName("gun_r")));
		meshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("gun_r"));
		isAttached = true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("FireComp Null"), true);
	}
}
