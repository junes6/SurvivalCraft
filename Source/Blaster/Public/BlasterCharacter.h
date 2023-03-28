// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "BlasterCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSetUpInputDelegate, class UEnhancedInputComponent*);

UCLASS(config=Game)
class ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

public:
	ABlasterCharacter();
	
	FSetUpInputDelegate SetUpInputDelegate;

	UPROPERTY(EditAnywhere)
	class USceneComponent* FPS_Scene;

	
	UPROPERTY(EditAnywhere)
	class USceneComponent* IronSight_Scene;

/*	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USceneComponent* ADSCamLoc*/;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditAnywhere)
	class UPlayerFireComponent* FireComponent;

	UPROPERTY()
	FVector FollowCamVector;
	////Ÿ�Ӷ��� ������Ʈ
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UTimelineComponent* timelineComp;

	////Ÿ�Ӷ��ο� ���� Ŀ�� �׷��� (�������Ʈ���� �ۼ���)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UCurveFloat* ADSFloat;

	////Ÿ�Ӷ����� ������ ����� �Լ��� ���ε��ϴ� ��������Ʈ
	//FOnTimelineEvent TimelineFinishedEvent;

	////Ÿ�Ӷ��� �߿� ����� �Լ��� ���ε��ϴ� ��������Ʈ
	//FOnTimelineFloat ADSReturnFunction;

	////Ÿ�Ӷ����� ������ ����� �Լ�
	//UFUNCTION()
	//void ADSTimelineFinish();

	////Ÿ�Ӷ����� �����Ű�� �Լ�
	//UFUNCTION()
	//void ADSTimelineStart();

	//ī�޶� �̵���Ű�� �Լ�
	//UFUNCTION()
	//void ADSCamMove(float alpha);

	////�����Լ�
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnADS();


};

