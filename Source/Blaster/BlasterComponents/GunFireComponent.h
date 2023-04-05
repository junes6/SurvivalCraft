// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunFireComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoCount, int32, ammoCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UGunFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunFireComponent();

	FOnAmmoCount AmmoCountDeleGate;
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

	UPROPERTY(Replicated)
	class ABlasterCharacter* player;

	UPROPERTY(EditAnywhere)
	int32 weaponPower = 5;

	UPROPERTY(EditAnywhere)
	int32 maxAmmo = 30;

	UPROPERTY(Replicated)
	int32 ammo;

	UFUNCTION(Server, Unreliable)
	void ServerReload();
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> fireShake;

	UPROPERTY()
	UCameraShakeBase* fireShakeIns;

	UFUNCTION()
	void Recoil();

	UPROPERTY()
	int32 recoilCount = 0;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* fireAnim;

	UFUNCTION(Server, Unreliable)
	void ServerSpawnBullet(FVector spawnLoc, FRotator spawnRot);

	UFUNCTION(NetMulticast, Unreliable)
	void MulitSpawnBullet(FVector spawnLoc, FRotator spawnRot);

	UPROPERTY(Replicated)
	FVector RepspawnLoc;

	UPROPERTY(Replicated)
	FRotator RepspawnRot;

	UFUNCTION(Server, Unreliable)
	void ServerPlayFireAnim(bool bADS);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayFireAnim(bool bADS);
};
