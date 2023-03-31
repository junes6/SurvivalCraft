// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include <Components/TextBlock.h>

void UOverheadWidget::SetDisplayText(FString TextToDisplay)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

// ShowPlayerNetRole을 호출하기 위해서는 Pawn을 전달하면된다.
void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	// 전달받은 Pawn의 LocalRole 데이터를 저장한다.
	ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;

	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	}

	// Pawn의 해당 Role을 LocalRoleString에 저장한다.
	FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);

	// LocalRoleString을 매개변수로 SetDisplayText에 전달한다.
	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::NativeDestruct()
{
	// 위젯 제거
	RemoveFromParent();
}



