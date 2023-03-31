// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSetUpWeapon, class AWeapon*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// friend class로 설정하면 private과 protected에 접근가능하다.
	friend class ABlasterCharacter;

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 무기 장착 함수(장착할 무기를 매개변수로 전달받는다.)
	void EquipWeapon(class AWeapon* WeaponToEquip);

	FSetUpWeapon setUpWeapon;

protected:
	virtual void BeginPlay() override;
	
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

private:
	// 캐릭터를 계속 캐스팅할 필요가 없다.
	class ABlasterCharacter* Character;

	// 장착된 무기
	UPROPERTY(Replicated)
	class AWeapon* EquippedWeapon;

	// Replicated를 통해 모든 클라이언트들이 볼 수 있도록 설정
	UPROPERTY(Replicated)
	bool bAiming;

public:	

	
		
};
