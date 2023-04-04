// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSetUpInputDelegate, class UInputComponent*);

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlasterCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���� ���� ��ҿ� ������ �� �ִ� �Լ�
	virtual void PostInitializeComponents() override;

	friend class UPlayerFireComponent;
protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void Lookup(float Value);
	void EquipButtonPressed();

	UPROPERTY(Replicated)
	int32 curHP;

	UPROPERTY(EditAnywhere)
	int32 maxHP = 100;

private:
	// �������� ������Ʈ �߰�
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* CameraBoom;

	// ī�޶� ������Ʈ �߰�
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* FollowCamera;

	// ���� �߰�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// �������� Ŭ���̾�Ʈ�� ����
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// Replicate�ɶ����� �ڵ����� ȣ��ȴ�.
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// ���� ������Ʈ �߰�
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	// RPC 
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

public:
	void SetOverlappingWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

	void CrouchButtonPressed();

	void AimPressed();

	void AimReleased();

	bool IsAiming();

	FSetUpInputDelegate SetUpInputDelegate;

	UPROPERTY(EditAnywhere)
	class USceneComponent* FPS_Scene;

	UPROPERTY(EditAnywhere)
	class USceneComponent* IronSight_Scene;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere)
	class UPlayerFireComponent* FireComponent;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* ADSGun;

	UPROPERTY()
	FVector FollowCamVector;

	UFUNCTION()
	FORCEINLINE int32 Get_curHP(){ return curHP; };

	UFUNCTION()
	void DecreaseHP(int32 value);

	UFUNCTION(Server, Unreliable)
	void ServerOnDamage(int32 value);
};
