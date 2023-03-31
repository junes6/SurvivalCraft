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
	
	// �ݸ����� �������̺�Ʈ ���ε�
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AOwnerShip::OnOverlap);

	// ���̽� ��Ƽ���� ����(�޽��� 0��° ��Ƽ����� ���ε�)
	BaseMaterial = Mesh->GetMaterial(0);

	if (BaseMaterial)
	{
		Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);

		// �޽ÿ� ���ο� ��Ƽ������ �����Ѵ�.
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
			// ������ �����ϰ� ����
			MatColor = FMath::VRand();
			ChangeColor();
		}
	}
}

void AOwnerShip::ChangeColor()
{
	// �÷�����
	Material->SetVectorParameterValue(FName("RandomColor"), (FLinearColor)MatColor);
}

// �Ű����� = ������ ���÷�����Ʈ ����Ҷ� �迭�� ��Ƽ� �������� ������ ������ �Ѱ��ش�.
void AOwnerShip::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ����ȭ�ϴ� ����(1. Ŭ�����̸�, 2. �����̸�)
	DOREPLIFETIME(AOwnerShip, MatColor);
}

