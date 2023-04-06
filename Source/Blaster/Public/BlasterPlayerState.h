// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BlasterPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FPlayerPointDele, float, float, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FSetTeamDele, int32);
/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:

	FPlayerPointDele PointDelegate;

	FSetTeamDele SetTeamDele;

	UPROPERTY(Replicated, ReplicatedUsing=OnRep_WidgetDeleGate)
	float KillCount = 0;

	UPROPERTY(Replicated)
	float DeathCount = 0;

	UPROPERTY(Replicated)
	float KDA = 0;

	UFUNCTION(Server, Unreliable)
	void SetKillCount(int32 value);

	UFUNCTION(Server, Unreliable)
	void SetDeathCount(int32 value);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiBroadCastDelegate();

	UFUNCTION()
	void calculateKDA();

	//값이 변경되면 위젯에 업데이트 한다
	UFUNCTION()
	void OnRep_WidgetDeleGate();

	//팀을 나눈다
	UPROPERTY(Replicated)
	int32 team;

	UFUNCTION(Server, Unreliable)
	void ServerSetTeam(int32 value);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiSetTeam(int32 value);
};



