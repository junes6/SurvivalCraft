// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerController.h"

#include "Blaster/HUD/PlayerUIWidget.h"

void ABlasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(playerUI && IsLocalController())
	{
		UPlayerUIWidget* playerUIWidget = CreateWidget<UPlayerUIWidget>(this, playerUI);
		playerUIWidget->AddToViewport();
	}
}
