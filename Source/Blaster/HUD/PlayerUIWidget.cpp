// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIWidget.h"

#include "BlasterGameState.h"
#include "BlasterPlayerState.h"
#include "Animation/UMGSequencePlayer.h"
#include "Blaster/BlasterComponents/GunFireComponent.h"
#include "Blaster/BlasterComponents/PlayerFireComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UPlayerUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	player = Cast<ABlasterCharacter>(GetOwningPlayerPawn());
	if(player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Widget player Cast"));
		player->UI = this;
	}
	

	//플레이어 스테이트를 캐스팅해서 델리게이트를 바인딩한다
	
	ps = Cast<ABlasterPlayerState>(player->GetPlayerState());
	if(ps)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("PointDele bind"));
		ps->PointDelegate.AddUObject(this, &UPlayerUIWidget::SetKillDeath);
	}

	WS_WidgetSwitcher->SetActiveWidgetIndex(0);
	text_KillCount->SetText(FText::AsNumber(0));
	text_DeathCount->SetText(FText::AsNumber(0));
	text_KDA->SetText(FText::AsNumber(0));

	ViewInfo(false);
}

void UPlayerUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!player)
	{

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Widget player NULL"));
		player = Cast<ABlasterCharacter>(GetOwningPlayerPawn());
		if (player)
		{
			player->UI = this;
		}
		return;
	}

	if(!ps)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Widget Player State NULL"));
		ps = Cast<ABlasterPlayerState>(player->GetPlayerState());
		if (ps)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("PointDele bind"));
			ps->PointDelegate.AddUObject(this, &UPlayerUIWidget::SetKillDeath);
		}
		return;
	}

	if(!gs)
	{
		gs = Cast<ABlasterGameState>(GetWorld()->GetGameState());
		return;
	}
	
	float percent = (float)player->Get_curHP() / (float)player->maxHP;
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("%f"), percent));
	text_HP->SetText(FText::AsNumber(player->Get_curHP()));
	text_Ammo->SetText(FText::AsNumber(player->ammo));

	pb_HP->SetPercent(percent);

	if(bUpdateInfo)
	{
		SetPlayerInfo();
	}
}

void UPlayerUIWidget::SetRespawnWidget()
{
	WS_WidgetSwitcher->SetActiveWidgetIndex(1);
	PlayAnimation(Anim_Respawn);
}

void UPlayerUIWidget::SetCombatWidget()
{
	WS_WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPlayerUIWidget::SetKillDeath(float kill, float death, float KDA)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("SetWidgetKDA"));
	//위젯에 킬뎃을 표시한다
	text_KillCount->SetText(FText::AsNumber(kill));
	text_DeathCount->SetText(FText::AsNumber(death));
	text_KDA->SetText(FText::AsNumber(KDA));
}

void UPlayerUIWidget::SetPlayerInfo()
{
	//게임 스테이트에서 KDA순으로 정렬된 플레이어리스트를 가져온다
	FString List = "";
	for (const auto& gps : gs->GetPlayerListByKDA())
	{
		FString name = gps->GetName();
		float kill = gps->KillCount;
		float Death = gps->DeathCount;
		float KDA = gps->KDA;

		List.Append(FString::Printf(TEXT("%s   kill : %.0f   Death : %.0f   KDA : %.2f\n"), *name, kill, Death, KDA));
	}
	text_PlayerList->SetText(FText::FromString(List));
}

void UPlayerUIWidget::ViewInfo(bool bShow)
{
	bUpdateInfo = bShow;
	if(bShow)
	{
		SetPlayerInfo();
		text_PlayerList->SetVisibility(ESlateVisibility::Visible);
		image_ListBackground->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{

		text_PlayerList->SetVisibility(ESlateVisibility::Hidden);
		image_ListBackground->SetVisibility(ESlateVisibility::Hidden);
	}
}
