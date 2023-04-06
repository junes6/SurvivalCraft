// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterPlayerController.h"
#include "BlasterGameInstance.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"

void ABlasterPlayerState::BeginPlay()
{
	Super::BeginPlay();

	/*UBlasterGameInstance* gi = Cast<UBlasterGameInstance>(GetGameInstance());
	if(gi)
	{
		gi->KillDelegate.AddUObject(this, &ABlasterPlayerState::SetKillCount);
	}*/
	//ABlasterCharacter player = Cast<ABlasterCharacter>(GetPlayerController()->GetCharacter());
	
}

void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterPlayerState, KillCount);
	DOREPLIFETIME(ABlasterPlayerState, DeathCount);
	DOREPLIFETIME(ABlasterPlayerState, KDA);
	DOREPLIFETIME(ABlasterPlayerState, team);
}

void ABlasterPlayerState::SetKillCount_Implementation(int32 value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SetK")));
	KillCount += value;
	calculateKDA();
	MultiBroadCastDelegate();
}

void ABlasterPlayerState::SetDeathCount_Implementation(int32 value)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SetD")));
	DeathCount += value;
	calculateKDA();
	//멀티 함수 실행
	MultiBroadCastDelegate();
}

//멀티로 실행한다
void ABlasterPlayerState::MultiBroadCastDelegate_Implementation()
{
	//네트워크 지연 고려
	FTimerHandle TimerHandle_PointDele;
	GetWorldTimerManager().SetTimer(TimerHandle_PointDele, FTimerDelegate::CreateLambda([&](){PointDelegate.Broadcast(KillCount, DeathCount, KDA); }), 1, false, 0);
	//PointDelegate.Broadcast(KillCount, DeathCount, KDA);
}

void ABlasterPlayerState::OnRep_WidgetDeleGate()
{
	//위젯에 정보를 넘긴다
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("OnRep_Widget")));
	//PointDelegate.Broadcast(KillCount, DeathCount);
}

void ABlasterPlayerState::MultiSetTeam_Implementation(int32 value)
{
	
}

void ABlasterPlayerState::ServerSetTeam_Implementation(int32 value)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ps Server Set Team"));

	//팀을 서버에서 설정한다
	team = value;
	//서버에서 델리게이트 호출 함수를 멀티캐스트한다
	MultiSetTeam(value);

	//플레이어가 델리게이트에 바인딩 될 때까지 기다린다
	FTimerHandle TimerHandle_SetTeam;
	GetWorldTimerManager().SetTimer(TimerHandle_SetTeam, FTimerDelegate::CreateLambda([&]()
		{
			SetTeamDele.Broadcast(value);
		}), 2.5, false, 0);
}

void ABlasterPlayerState::calculateKDA()
{
		KDA = KillCount / (DeathCount+1);
}
