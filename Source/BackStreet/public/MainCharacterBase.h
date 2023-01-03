// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "CharacterBase.h"
#include "GameFramework/Character.h"
#include "MainCharacterBase.generated.h"

UCLASS()
class BACKSTREET_API AMainCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ------- Character Action ------- 
public:
	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void Dash();

	UFUNCTION(BlueprintImplementableEvent)
		void Roll();

	UFUNCTION()
		virtual void TryReload() override;

	UFUNCTION(BlueprintCallable)
		virtual void TryAttack() override;

	UFUNCTION(BlueprintCallable)
		virtual void Attack() override;

	UFUNCTION(BlueprintCallable)
		virtual void StopAttack() override;

// -------- 자원 관리 ---------
public:
	//UFUNCTION()
		virtual void ClearAllTimerHandle() override;

public:
	//플레이어 메인 카메라 붐
	UPROPERTY(VisibleDefaultsOnly)
		USpringArmComponent* CameraBoom;
	
	//플레이어의 메인 카메라
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		UCameraComponent* FollowingCamera;

private:
	UPROPERTY()
		class AMainCharacterController* PlayerControllerRef;

	//공격 시, 마우스 커서의 위치로 캐릭터가 바라보는 로직을 초기화하는 타이머
	//초기화 시에는 다시 Movement 방향으로 캐릭터의 Rotation을 Set
	UPROPERTY()
		FTimerHandle RotationFixTimerHandle;
};
