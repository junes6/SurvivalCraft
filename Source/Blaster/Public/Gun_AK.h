// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunFireComponent.h"
#include "GameFramework/Actor.h"
#include "Gun_AK.generated.h"

UCLASS()
class BLASTER_API AGun_AK : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun_AK();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* meshComp;

	UFUNCTION()
	void GunFireStart();

	UFUNCTION()
	void GunFireStop();
protected:

	UPROPERTY(EditAnywhere)
	class UGunFireComponent* GunFire;

	UFUNCTION()
	void AttachGun();

	UPROPERTY()
	bool isAttached;
	
};


