// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OwnerShip.generated.h"

UCLASS()
class BLASTER_API AOwnerShip : public AActor
{
	GENERATED_BODY()
	
public:	
	AOwnerShip();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereCollision;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(ReplicatedUsing = ChangeColor)
	FVector MatColor;

	UPROPERTY()
	class UMaterialInterface* BaseMaterial;

	UPROPERTY()
	class UMaterialInstanceDynamic* Material;

	UFUNCTION()
	void ChangeColor();

};
