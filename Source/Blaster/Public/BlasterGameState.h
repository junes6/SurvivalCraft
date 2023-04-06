// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BlasterGameState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:

	TArray<class ABlasterPlayerState*> GetPlayerListByKDA();

	UPROPERTY()
	TArray<class ABlasterPlayerState*> BlasterPlayerList;

	UPROPERTY(Replicated)
	int32 team1Count;

	UPROPERTY(Replicated)
	int32 team2Count;
};
