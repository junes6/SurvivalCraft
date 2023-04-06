// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"

#include "BlasterPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

void ABlasterGameState::BeginPlay()
{
	Super::BeginPlay();

	
	/*for (auto playerState : PlayerArray)
	{
		ABlasterPlayerState* bps = Cast<ABlasterPlayerState>(playerState);
		if(bps)
		{
			BlasterPlayerList.Add(bps);
		}
	}*/
}

TArray<ABlasterPlayerState*> ABlasterGameState::GetPlayerListByKDA()
{
	//게임모드에 있는 플레이어스테이트를 블레스터플레이어 스테이트로 캐스팅해서 배열에 담는다
	BlasterPlayerList.Empty();
	for (auto playerState : PlayerArray)
	{
		ABlasterPlayerState* bps = Cast<ABlasterPlayerState>(playerState);
		if (bps)
		{
			BlasterPlayerList.Add(bps);
		}
	}

	TArray<ABlasterPlayerState*> SortBlasterPlayerList = BlasterPlayerList;

	//KDA가 높은 순으로 정렬해서 반환한다
	SortBlasterPlayerList.Sort([](const ABlasterPlayerState& a, const ABlasterPlayerState& b) { return a.KDA > b.KDA; });
	return SortBlasterPlayerList;


}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, team1Count);
	DOREPLIFETIME(ABlasterGameState, team2Count);
}