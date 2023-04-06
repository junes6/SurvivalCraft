// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerUIWidget> playerUI;

	UPROPERTY()
	class ABlasterPlayerState* ps;

	UPROPERTY()
	class ABlasterGameState* gs;

	UPROPERTY()
	class UPlayerUIWidget* playerUIWidget;

	UFUNCTION()
	void Respawn(class ABlasterCharacter* player);
};
