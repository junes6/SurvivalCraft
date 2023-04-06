// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun_Bullet.h"

#include "Blaster/BlasterComponents/GunFireComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	projectile->bRotationFollowsVelocity = true;

	//meshComp->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AGun_Bullet::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5);

	meshComp->OnComponentBeginOverlap.AddDynamic(this, &AGun_Bullet::OnBulletOverlap);
	meshComp->OnComponentHit.AddDynamic(this, &AGun_Bullet::DestroyBullet);

	FString myLocalRole = UEnum::GetValueAsString<ENetRole>(GetLocalRole());
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, myLocalRole);
}

// Called every frame
void AGun_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		FHitResult hitResult;
		
		params.AddIgnoredActor(this);
		if(gunFireComp)
		{
			params.AddIgnoredActor(gunFireComp->GetOwner());
		}
		if(GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation()+GetActorForwardVector()*150, ECC_GameTraceChannel2, params))
		{
			ABlasterCharacter* player = Cast<ABlasterCharacter>(hitResult.GetActor());
			if(player)
			{
				if (gunFireComp)
				{
					player->ServerOnDamage(gunFireComp->weaponPower, Cast<ABlasterCharacter>(GetOwner()));
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("trace"));
				}
				//총알이 플레이어에게 데미지를 주면 ignoreActor를 추가해서 상호작용 x
				params.AddIgnoredActor(player);
			}
		}
	}
}

void AGun_Bullet::Destroyed()
{
	Super::Destroyed();

	if(BulletDestorySystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletDestorySystem, hitLocation, -1*GetActorRotation());
	}
}

void AGun_Bullet::ServerPlayerDamage_Implementation(ABlasterCharacter* player)
{
	if(player&&gunFireComp)
	{
		//player->DecreaseHP(gunFireComp->weaponPower);
	}
}

void AGun_Bullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlasterCharacter* player = Cast<ABlasterCharacter>(OtherActor);

	if (player)
	{
		//플레이어 데미지
		//ServerPlayerDamage(player);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("No Player"));
	}
}

void AGun_Bullet::DestroyBullet(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//block되면 총알을 삭제한다
	hitLocation =  Hit.Location;

	Destroy();
}
