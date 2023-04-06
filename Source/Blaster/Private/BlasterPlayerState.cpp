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
	//��Ƽ �Լ� ����
	MultiBroadCastDelegate();
}

//��Ƽ�� �����Ѵ�
void ABlasterPlayerState::MultiBroadCastDelegate_Implementation()
{
	//��Ʈ��ũ ���� ���
	FTimerHandle TimerHandle_PointDele;
	GetWorldTimerManager().SetTimer(TimerHandle_PointDele, FTimerDelegate::CreateLambda([&](){PointDelegate.Broadcast(KillCount, DeathCount, KDA); }), 1, false, 0);
	//PointDelegate.Broadcast(KillCount, DeathCount, KDA);
}

void ABlasterPlayerState::OnRep_WidgetDeleGate()
{
	//������ ������ �ѱ��
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("OnRep_Widget")));
	//PointDelegate.Broadcast(KillCount, DeathCount);
}

void ABlasterPlayerState::MultiSetTeam_Implementation(int32 value)
{
	
}

void ABlasterPlayerState::ServerSetTeam_Implementation(int32 value)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("ps Server Set Team"));

	//���� �������� �����Ѵ�
	team = value;
	//�������� ��������Ʈ ȣ�� �Լ��� ��Ƽĳ��Ʈ�Ѵ�
	MultiSetTeam(value);

	//�÷��̾ ��������Ʈ�� ���ε� �� ������ ��ٸ���
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
