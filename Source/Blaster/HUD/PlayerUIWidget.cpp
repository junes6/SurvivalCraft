// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerUIWidget.h"

#include "Blaster/BlasterComponents/GunFireComponent.h"
#include "Blaster/BlasterComponents/PlayerFireComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	player = Cast<ABlasterCharacter>(GetOwningPlayerPawn());
	if(player)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Emerald, TEXT("Widget player Cast"));
	}

}

void UPlayerUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!player){ return; }

	float percent = (float)player->Get_curHP() / (float)player->maxHP;
	// GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Cyan, FString::Printf(TEXT("%f"), percent));
	text_HP->SetText(FText::AsNumber(player->Get_curHP()));
	text_Ammo->SetText(FText::AsNumber(player->ammo));

	pb_HP->SetPercent(percent);
}
