// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameMode.h"

#include "EngineUtils.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AActor* ABlasterGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<APlayerStart> iter(GetWorld()); iter; ++iter)
	{
		TArray<AActor*> players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlasterCharacter::StaticClass(), players);

		APlayerStart* ps = *iter;

		bool bClose = false;
		//플레이어와의 거리를 구하고 거리가 500보다 작으면 거른다
		for(auto player : players)
		{
			float distance = ps->GetDistanceTo(player);
			if(distance <= 500)
			{
				//거리가 500보다 작은 플레이어가 있다면 참으로한다
				bClose = true;
			}
		}

		//거리가 좁은 플레이어가 없다면 리턴하고 함수 종료
		if(!bClose)
		{
			return ps;
		}
	}

	//조건에 맞는 ps가 없다면 자동실행
	return Super::ChoosePlayerStart_Implementation(Player);
}

