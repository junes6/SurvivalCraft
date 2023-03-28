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
	////타임라인 컴포넌트
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UTimelineComponent* timelineComp;

	////타임라인에 사용될 커브 그래프 (블루프린트에서 작성됨)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UCurveFloat* ADSFloat;

	////타임라인이 끝나면 실행될 함수를 바인딩하는 델리게이트
	//FOnTimelineEvent TimelineFinishedEvent;

	////타임라인 중에 실행될 함수를 바인딩하는 델리게이트
	//FOnTimelineFloat ADSReturnFunction;

	////타임라인이 끝나면 실행될 함수
	//UFUNCTION()
	//void ADSTimelineFinish();

	////타임라인을 실행시키는 함수
	//UFUNCTION()
	//void ADSTimelineStart();

	//카메라를 이동시키는 함수
	//UFUNCTION()
	//void ADSCamMove(float alpha);

	////조준함수
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnADS();


};

