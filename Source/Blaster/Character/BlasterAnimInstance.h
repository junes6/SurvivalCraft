// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// 게임시작과 동시에 작동(= BeginPlay)
	virtual void NativeInitializeAnimation() override;
	// 매 프레임마다 작동(= TickEvent)
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = true))
	class ABlasterCharacter* BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	bool bWeaponEquipped;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = true))
	float playerAimPitch;

	UFUNCTION()
	void AnimNotify_ReloadingEnd();

public:
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	bool bDie;
};
