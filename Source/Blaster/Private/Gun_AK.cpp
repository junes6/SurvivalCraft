// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_AK.h"

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

	GunFire = CreateDefaultSubobject<UGunFireComponent>(TEXT("GunFire"));
}

// Called when the game starts or when spawned
void AGun_AK::BeginPlay()
{
	Super::BeginPlay();
	AttachGun();
}

// Called every frame
void AGun_AK::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGun_AK::GunFireStart()
{
	if (!GunFire) { return; }
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
	float maxDistance = 500;
	for(TActorIterator<ABlasterCharacter> it(GetWorld()); it; ++it)
	{
		float distance = GetDistanceTo(*it);

		if(distance < maxDistance)
		{
			maxDistance = distance;
			player = *it;
		}
	}

	//�÷��̾ �ְ� ������Ʈ�� 
	if(player&&player->FireComp)
	{
		player->FireComp->GetAK(this);
		USkeletalMeshComponent* playerMesh = player->GetMesh();
		this->SetActorLocationAndRotation(playerMesh->GetSocketLocation(FName("gun_r")), playerMesh->GetSocketRotation(FName("gun_r")));
		meshComp->AttachToComponent(playerMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("gun_r"));
	}
}
