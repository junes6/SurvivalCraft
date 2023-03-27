// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_Bullet.h"

#include "BlasterCharacter.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGun_Bullet::AGun_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	SetRootComponent(meshComp);

	projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovement"));
	projectile->InitialSpeed = 2000.f;
	projectile->MaxSpeed = 2000.f;
	/*ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'"));
	if (TempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(TempMesh.Object);
	}*/

}

// Called when the game starts or when spawned
void AGun_Bullet::BeginPlay()
{
	Super::BeginPlay();

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &AGun_Bullet::OnBulletOverlap);
	
}

// Called every frame
void AGun_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun_Bullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlasterCharacter* player = Cast<ABlasterCharacter>(OtherActor);

	if(player)
	{
		//플레이어 데미지
	}
}
