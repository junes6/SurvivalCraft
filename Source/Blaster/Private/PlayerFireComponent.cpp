// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"

#include "BlasterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Gun_AK.h"
#include "Camera/CameraComponent.h"

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
	// ...
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
	ak = getAk;
}

void UPlayerFireComponent::InputFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("fire"), true);
	if(ak)
	{
		ak->GunFireStart();
	}
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
		//ADS 카메라를 조준점으로 옮기면서 활성화 한다
		me->ADSCamera->SetRelativeLocation(ak->meshComp->GetSocketLocation(FName("ADS")));
		me->ADSCamera->SetActive(true);
		me->GetFollowCamera()->SetActive(false);
	}
}

void UPlayerFireComponent::OffADS()
{
	//ak가 할당 되어 있다면
	if (ak)
	{
		//ADS 카매라를 원래 위치로 옮기고 완료되면 3인칭 카매라를 활성화한다
		
	}
}

