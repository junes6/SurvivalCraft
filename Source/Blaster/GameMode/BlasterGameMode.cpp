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
		//�÷��̾���� �Ÿ��� ���ϰ� �Ÿ��� 500���� ������ �Ÿ���
		for(auto player : players)
		{
			float distance = ps->GetDistanceTo(player);
			if(distance <= 500)
			{
				//�Ÿ��� 500���� ���� �÷��̾ �ִٸ� �������Ѵ�
				bClose = true;
			}
		}

		//�Ÿ��� ���� �÷��̾ ���ٸ� �����ϰ� �Լ� ����
		if(!bClose)
		{
			return ps;
		}
	}

	//���ǿ� �´� ps�� ���ٸ� �ڵ�����
	return Super::ChoosePlayerStart_Implementation(Player);
}

