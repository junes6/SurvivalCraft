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
	//���Ӹ�忡 �ִ� �÷��̾����Ʈ�� �������÷��̾� ������Ʈ�� ĳ�����ؼ� �迭�� ��´�
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

	//KDA�� ���� ������ �����ؼ� ��ȯ�Ѵ�
	SortBlasterPlayerList.Sort([](const ABlasterPlayerState& a, const ABlasterPlayerState& b) { return a.KDA > b.KDA; });
	return SortBlasterPlayerList;


}

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, team1Count);
	DOREPLIFETIME(ABlasterGameState, team2Count);
}