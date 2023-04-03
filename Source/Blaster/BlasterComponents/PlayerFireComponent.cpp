// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"

#include "CombatComponent.h"
#include "GunFireComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UPlayerFireComponent::UPlayerFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

	cam = me->GetFollowCamera();

	me->bUseControllerRotationYaw = true;

	//무기 가져오기 델리게이트 바인딩
	me->Combat->setUpWeapon.AddUObject(this, &UPlayerFireComponent::GetAK);
}


// Called every frame
void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bADS)
	{
		//me->ADSGun->SetRelativeRotation(FRotator(0, 0, -cam->GetRelativeRotation().Pitch));
		cam->SetWorldLocation(me->ADSGun->GetSocketLocation(FName("ADS")));
	}
	
	// ...
}

void UPlayerFireComponent::SetupInputComponent(UInputComponent* EnhancedInputComponent)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("bind"), true);

	EnhancedInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFireComponent::InputFire);
	EnhancedInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &UPlayerFireComponent::InputFireCompeleted);
	EnhancedInputComponent->BindAction(TEXT("ADS"), IE_Pressed, this, &UPlayerFireComponent::OnADS);
	EnhancedInputComponent->BindAction(TEXT("ADS"), IE_Released, this, &UPlayerFireComponent::OffADS);
}

void UPlayerFireComponent::InitializeComponent()
{
	Super::InitializeComponent();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("init"), true);
	me = Cast<ABlasterCharacter>(GetOwner());

	me->SetUpInputDelegate.AddUObject(this, &UPlayerFireComponent::SetupInputComponent);
}

void UPlayerFireComponent::GetAK(AWeapon* getAk, ABlasterCharacter* player)
{
	if (!getAk)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Get Weapon Failed"), true);
	}
	else
	{
		ak = getAk;
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Get AK"), true);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("%s"), *ak->GetName()), true);

		//ak에 Owner를 넣는다
		ak->SetOwner(player);
		ak->GunFire->player = player;
	}
}

//void UPlayerFireComponent::GetAK(AWeapon* getAk)
//{
//	//if(me&&me->GetController()&&me->GetController()->IsLocalController())
//	//{
//	if(!getAk)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Get Weapon Failed"), true);
//	}
//	else
//	{
//		ak = getAk;
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Get AK"), true);
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("%s"), *ak->GetName()), true);
//	}
//
//		
//	//}
//}

void UPlayerFireComponent::InputFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("fireInput"), true);
	if (ak)
	{
		ak->GunFireStart();
	}
	else { GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("ak Null"), true); }
}

void UPlayerFireComponent::InputFireCompeleted()
{
	if (ak)
	{
		ak->GunFireStop();
	}
}

void UPlayerFireComponent::OnADS()
{
	//ak가 할당 되어 있다면

	if (ak)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("OnADS"), true);
		//1인칭 뷰로 전환
		//me->GetCameraBoom()->SetActive(false);

		//조준 시작을 참으로
		bADS_Start = true;
		//카메라의 회전을 막아서 설정한 대로 돌아갈수 있게 한다
		me->GetCameraBoom()->SetActive(false);
		//cam->bUsePawnControlRotation = false;
		cam->SetRelativeLocation(me->FPS_Scene->GetRelativeLocation());
		cam->SetWorldRotation(me->GetActorRotation());
		

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

		//me->bUseControllerRotationYaw = false;
		//카메라를 회전하게 한다
		cam->bUsePawnControlRotation = true;
		//스프링암을 활성화 한다
		me->GetCameraBoom()->SetActive(true);
		me->GetMesh()->SetOwnerNoSee(false);
		//me->bUseControllerRotationPitch = false;
		//ADS 상태를 비활성화한다
		bADS = false;
		bADS_Start = false;
	}
}

void UPlayerFireComponent::SmoothADS()
{
	alpha += 0.01;
	cam->SetWorldLocation(FMath::Lerp(cam->GetComponentLocation(), me->ADSGun->GetSocketLocation(FName("ADS")), alpha));
	if (alpha >= 1)
	{
		alpha = 0;
		GetWorld()->GetTimerManager().ClearTimer(ADS_Timer);

		//카메라 회전가능
		cam->bUsePawnControlRotation = true;
		bADS = true;

		//자기 캐릭터의 매쉬가 조준중에는 안보이게 한다
		me->GetMesh()->SetOwnerNoSee(true);

		//me->ADSGun->AttachToComponent(c);
		//카메라가 상하좌우로 회전하게 한다
		//me->bUseControllerRotationYaw = true;
		//me->bUseControllerRotationPitch = true;
	}
}

void UPlayerFireComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerFireComponent, ak);
}