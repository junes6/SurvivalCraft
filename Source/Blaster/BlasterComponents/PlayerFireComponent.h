// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UPlayerFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFireComponent();

	//friend class UGunFireComponent;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputComponent(class UInputComponent* EnhancedInputComponent);

	//비긴플레이보다 빠르게 실행 델리게이트 연결
	virtual void InitializeComponent() override;

	UFUNCTION()
	void GetAK(class AWeapon* getAk);
protected:

	UPROPERTY()
	class ABlasterCharacter* me;

	UPROPERTY()
	class AWeapon* ak;

	UFUNCTION()
	void InputFire();

	UFUNCTION()
	void InputFireCompeleted();

	UFUNCTION()
	void OnADS();

	UFUNCTION()
	void OffADS();

	UPROPERTY()
	FTimerHandle ADS_Timer;

	UFUNCTION()
	void SmoothADS();

	UPROPERTY()
	class UCameraComponent* cam;

	UPROPERTY()
	float alpha;

public:

	UPROPERTY();
	bool bADS = false;
};
