// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"

#include "BlasterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Gun_AK.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UPlayerFireComponent::UPlayerFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	bWantsInitializeComponent = true;

}


// Called when the game starts
void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();
	cam = me->GetFollowCamera();
	// ...
	//소유자에게서 timelineComp를 받아온다
}


// Called every frame
void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("init"), true);
	// ...
}

void UPlayerFireComponent::SetupInputComponent(UEnhancedInputComponent* EnhancedInputComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("bind"), true);
	EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &UPlayerFireComponent::InputFire);
	EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &UPlayerFireComponent::InputFireCompeleted);
	EnhancedInputComponent->BindAction(IA_ADS, ETriggerEvent::Started, this, &UPlayerFireComponent::OnADS);
	EnhancedInputComponent->BindAction(IA_ADS, ETriggerEvent::Completed, this, &UPlayerFireComponent::OffADS);
}

void UPlayerFireComponent::InitializeComponent()
{
	Super::InitializeComponent();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("init"), true);
	me = Cast<ABlasterCharacter>(GetOwner());

	me->SetUpInputDelegate.AddUObject(this, &UPlayerFireComponent::SetupInputComponent);
}

void UPlayerFireComponent::GetAK(AGun_AK* getAk)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Get AK"), true);
	ak = getAk;
}

void UPlayerFireComponent::InputFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("fireInput"), true);
	if(ak)
	{
		ak->GunFireStart();
	}
	else{ GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("ak Null"), true); }
}

void UPlayerFireComponent::InputFireCompeleted()
{
	if(ak)
	{
		ak->GunFireStop();
	}
}

void UPlayerFireComponent::OnADS()
{
	//ak가 할당 되어 있다면
	
	if(ak)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("OnADS"), true);
		//1인칭 뷰로 전환
		//me->GetCameraBoom()->SetActive(false);
		cam->SetRelativeLocation(me->FPS_Scene->GetRelativeLocation());
		GetWorld()->GetTimerManager().SetTimer(ADS_Timer, this, &UPlayerFireComponent::SmoothADS, 0.01, true, 0);
		//cam->SetWorldLocation(ak->meshComp->GetSocketLocation(FName("ADS")));
		
	}
}

void UPlayerFireComponent::OffADS()
{
	//ak가 할당 되어 있다면
	if (ak)
	{
		//me->GetCameraBoom()->SetActive(true);
		alpha = 0;
		GetWorld()->GetTimerManager().ClearTimer(ADS_Timer);
		cam->SetWorldLocation(me->IronSight_Scene->GetComponentLocation());
	}
}

void UPlayerFireComponent::SmoothADS()
{
	alpha += 0.01;
	cam->SetWorldLocation(FMath::Lerp(cam->GetComponentLocation(), ak->meshComp->GetSocketLocation(FName("ADS")), alpha));
	if(alpha >= 1)
	{
		alpha = 0;
		GetWorld()->GetTimerManager().ClearTimer(ADS_Timer);
	}
}

