// Fill out your copyright notice in the Description page of Project Settings.


#include "OwnerShip.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AOwnerShip::AOwnerShip()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

}

void AOwnerShip::BeginPlay()
{
	Super::BeginPlay();
	
	// 콜리전과 오버랩이벤트 바인딩
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AOwnerShip::OnOverlap);

	// 베이스 머티리얼 설정(메시의 0번째 머티리얼로 바인딩)
	BaseMaterial = Mesh->GetMaterial(0);

	if (BaseMaterial)
	{
		Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);

		// 메시에 새로운 머티리얼을 적용한다.
		Mesh->SetMaterial(0, Material);
	}
}

void AOwnerShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOwnerShip::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(HasAuthority())
	{
		ABlasterCharacter* PlayerCharacter = Cast<ABlasterCharacter>(OtherActor);

		if (PlayerCharacter)
		{
			// 색깔을 랜덤하게 변경
			MatColor = FMath::VRand();
			ChangeColor();
		}
	}
}

void AOwnerShip::ChangeColor()
{
	// 컬러변경
	Material->SetVectorParameterValue(FName("RandomColor"), (FLinearColor)MatColor);
}

// 매개변수 = 서버에 리플레케이트 등록할때 배열에 담아서 여러개의 변수를 서버에 넘겨준다.
void AOwnerShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 동기화하는 간격(1. 클래스이름, 2. 변수이름)
	DOREPLIFETIME(AOwnerShip, MatColor);
}

