// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "GameFramework/Actor.h"
#include "WeaponStatStructBase.h"
#include "ProjectileBase.generated.h"


UCLASS()
class BACKSTREET_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void InitProjectile(FProjectileStatStruct NewStat, class ACharacterBase* NewCharacterRef);

	UFUNCTION()
		void OnProjectileBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
								, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnTargetBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
			, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void ActivateProjectileMovement();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		ACharacterBase* GetOwnerCharacterRef() { return OwnerCharacterRef; }

public:	
	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* SphereCollision;

	UPROPERTY(VisibleDefaultsOnly)
		USphereComponent* TargetingCollision;

	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* Mesh; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class UProjectileMovementComponent* ProjectileMovement;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
		UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay|Stat")
		struct FProjectileStatStruct ProjectileStat;

private: 
	UPROPERTY()
		bool bIsActivated = false;

	UPROPERTY()
		AController* SpawnInstigator;

	UPROPERTY()
		class ACharacterBase* OwnerCharacterRef;
};
