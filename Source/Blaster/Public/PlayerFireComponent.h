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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputComponent(class UEnhancedInputComponent* EnhancedInputComponent);

	//����÷��̺��� ������ ���� ��������Ʈ ����
	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Fire;

	UFUNCTION()
	void GetAK(class AGun_AK* getAk);
protected:

	UPROPERTY()
	class ABlasterCharacter* me;

	UPROPERTY()
	class AGun_AK* ak;

	UFUNCTION()
	void InputFire();

	UFUNCTION()
	void InputFireCompeleted();

		
};


