// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunFireComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// friend class로 설정하면 private과 protected에 접근가능하다.
	//friend class AWeapon;

	UFUNCTION()
	void GunFireStart();

	UFUNCTION()
	void GunFireStop();
protected:
	UPROPERTY()
	class AWeapon* me;

	UPROPERTY()
	class USkeletalMeshComponent* gunMesh;

	UPROPERTY()
	FTimerHandle TimerHandle_AutoFire;

	UFUNCTION()
	void GunFire();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AGun_Bullet> bullet;

	UFUNCTION()
	void SpawnMuzzleEffect();

	UPROPERTY()
	APlayerCameraManager* localPlayerCamManager;

	UPROPERTY()
	class ABlasterCharacter* player;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> fireShake;

	UPROPERTY()
	UCameraShakeBase* fireShakeIns;

	UFUNCTION()
	void Recoil();

	UPROPERTY()
	int32 recoilCount = 0;
};
