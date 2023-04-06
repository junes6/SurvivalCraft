// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UPlayerUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* text_HP;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* text_Ammo;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* text_DeathCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* text_KillCount;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* text_KDA;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* text_PlayerList;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* pb_HP;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UWidgetSwitcher* WS_WidgetSwitcher;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UProgressBar* pb_respawn;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	class UImage* image_ListBackground;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* Anim_Respawn;

	UPROPERTY()
	class ABlasterCharacter* player;

	UPROPERTY()
	class ABlasterPlayerState* ps;

	UPROPERTY()
	class ABlasterGameState* gs;

	UFUNCTION()
	void SetRespawnWidget();

	UFUNCTION()
	void SetCombatWidget();

	UFUNCTION()
	void SetKillDeath(float kill, float death, float KDA);

	UFUNCTION()
	void SetPlayerInfo();

	UFUNCTION()
	void ViewInfo(bool bShow);

	UPROPERTY()
	bool bUpdateInfo;
};
