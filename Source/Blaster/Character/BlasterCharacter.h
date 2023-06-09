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

	// 전투 구성 요소에 접근할 수 있는 함수
	virtual void PostInitializeComponents() override;

	friend class UPlayerFireComponent;
protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void Lookup(float Value);
	void EquipButtonPressed();

private:
	// 스프링암 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* CameraBoom;

	// 카메라 컴포넌트 추가
	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* FollowCamera;

	// 위젯 추가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	// 서버에서 클라이언트로 복제
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	// Replicate될때마다 자동으로 호출된다.
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	// 전투 컴포넌트 추가
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
};
