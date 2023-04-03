// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(Replicated, EditDefaultsOnly, BluePrintReadOnly, Category = AnimSetting)
	float weaponPitch = 0;

	UPROPERTY()
	class ABlasterCharacter* player;

};
