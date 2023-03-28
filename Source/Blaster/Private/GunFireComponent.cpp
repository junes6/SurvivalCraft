// Fill out your copyright notice in the Description page of Project Settings.


#include "GunFireComponent.h"

#include "Gun_AK.h"
#include "Gun_Bullet.h"

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
	me = Cast<AGun_AK>(GetOwner());
	gunMesh = Cast<USkeletalMeshComponent>(me->meshComp);
}


// Called every frame
void UGunFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunFireComponent::GunFireStart()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutoFire, this, &UGunFireComponent::GunFire, 0.2, true);
}

void UGunFireComponent::GunFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("fire"), true);
	GetWorld()->SpawnActor<AGun_Bullet>(bullet, gunMesh->GetSocketLocation(FName("Muzzle")), gunMesh->GetSocketRotation(FName("Muzzle")));
}

void UGunFireComponent::GunFireStop()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("endFire"), true);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutoFire);
}


