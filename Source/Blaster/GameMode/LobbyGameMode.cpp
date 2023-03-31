// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// GameMode 내부에 있는 GameState에서 현재 접속한 플레이어 숫자를 저장
	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

	// 만약 접속한 플레이어가 2명이면
	if (NumberOfPlayers == 2)
	{
		UWorld* World = GetWorld();

		if (World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
