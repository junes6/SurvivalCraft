// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"

#include "BlasterCharacter.h"
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
}
