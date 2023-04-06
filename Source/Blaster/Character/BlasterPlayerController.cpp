// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"

#include "BlasterCharacter.h"
#include "BlasterGameState.h"
#include "BlasterPlayerState.h"
#include "Blaster/GameMode/BlasterGameMode.h"
#include "Blaster/HUD/PlayerUIWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ABlasterCharacter* player = Cast<ABlasterCharacter>(GetPawn());

	if(playerUI && IsLocalController())
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("AddWidget"), true);
		playerUIWidget = CreateWidget<UPlayerUIWidget>(this, playerUI);
		playerUIWidget->AddToViewport();
		playerUIWidget->text_HP->SetText(FText::AsNumber(player->maxHP));
		playerUIWidget->pb_HP->SetPercent(player->Get_curHP()/player->maxHP);
	}

	//로컬에서 팀을 정한다
	if(HasAuthority())
	{
		gs = Cast<ABlasterGameState>(GetWorld()->GetGameState());
		ps = GetPlayerState<ABlasterPlayerState>();
		if (ps && gs)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Controller SetTeam"));
			if (gs->team1Count <= gs->team2Count)
			{
				gs->team1Count++;
				ps->ServerSetTeam(1);
			}
			else
			{
				gs->team2Count++;
				ps->ServerSetTeam(2);
			}
		}
	}
	
}

void ABlasterPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (HasAuthority()&& (!gs || !ps))
	{
		if(!ps)
		{
			ps = GetPlayerState<ABlasterPlayerState>();
		}
		if(!gs)
		{
			gs = Cast<ABlasterGameState>(GetWorld()->GetGameState());
		}
		if(gs && ps)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Controller SetTeam"));
			if (gs->team1Count <= gs->team2Count)
			{
				ps->ServerSetTeam(1);
			}
			else
			{
				ps->ServerSetTeam(2);
			}
		}
	}
}

//캐릭터 부활 함수
void ABlasterPlayerController::Respawn(ABlasterCharacter* player)
{
	//서버에서 실행 게임모드는 서버만 있다
	if (HasAuthority() && player)
	{
		ABlasterGameMode* gm = Cast<ABlasterGameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			player->Destroy();

			gm->RestartPlayerAtPlayerStart(this, gm->ChoosePlayerStart_Implementation(this));

			ps->SetTeamDele.Broadcast(ps->team);
		}
	}
}
