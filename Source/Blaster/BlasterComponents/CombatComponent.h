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

	// friend class�� �����ϸ� private�� protected�� ���ٰ����ϴ�.
	friend class ABlasterCharacter;

	// virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���� ���� �Լ�(������ ���⸦ �Ű������� ���޹޴´�.)
	void EquipWeapon(class AWeapon* WeaponToEquip);

	FSetUpWeapon setUpWeapon;

protected:
	virtual void BeginPlay() override;
	
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

private:
	// ĳ���͸� ��� ĳ������ �ʿ䰡 ����.
	class ABlasterCharacter* Character;

	// ������ ����
	UPROPERTY(Replicated)
	class AWeapon* EquippedWeapon;

	// Replicated�� ���� ��� Ŭ���̾�Ʈ���� �� �� �ֵ��� ����
	UPROPERTY(Replicated)
	bool bAiming;

public:	

	
		
};
