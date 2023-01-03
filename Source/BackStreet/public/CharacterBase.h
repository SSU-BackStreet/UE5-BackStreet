// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "CharacterInfoStructBase.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class BACKSTREET_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//----- 오버라이트 함수 ----------
public:
	// Sets default values for this character's properties
	ACharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Input에 Binding 되어 공격을 시도 (AnimMontage를 호출)
	virtual void TryAttack();

	//AnimNotify에 Binding 되어 실제 공격을 수행
	virtual void Attack();

	virtual void StopAttack();

	virtual void TryReload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

// ------- 캐릭터 컴포넌트 -------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UChildActorComponent* WeaponActor;

// ------- Character Action 기본 ------- 
public:
	//플레이어가 현재 해당 Action을 수행하고 있는지 반환
	UFUNCTION(BlueprintCallable)
		bool GetIsActionActive(ECharacterActionType Type) { return CharacterState.CharacterActionState == Type; }
	
	//플레이어의 ActionState를 Idle로 전환한다.
	UFUNCTION(BlueprintCallable)
		void ResetActionState();

	UFUNCTION()
		virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent
			, AController* EventInstigator, AActor* DamageCauser) override;

	//디버프 데미지를 입힘 (일회성)
	UFUNCTION()
		float TakeDebuffDamage(float DamageAmount, uint8 DebuffType, AActor* Causer);

	//플레이어가 체력을 회복함 (일회성)
	UFUNCTION()
		void TakeHeal(float HealAmount, bool bIsTimerEvent = false, uint8 BuffType = 0);

	UFUNCTION()
		void Die();

// ------- Character Stat/State ------- 
public:
	//캐릭터의 상태 정보를 초기화
	UFUNCTION()
		void InitCharacterState();

	//캐릭터의 스탯을 업데이트
	UFUNCTION(BlueprintCallable)
		void UpdateCharacterStat(FCharacterStatStruct NewStat);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		FCharacterStatStruct GetCharacterStat() { return CharacterStat; }

// ------ 무기 관련 ----------------
public:
	//UFUNCTION()
		//void ChangeWeapon();

	//무기 관련 설정을 초기화
	UFUNCTION()
		void InitWeapon();

	//무기 Ref를 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		class AWeaponBase* GetWeaponActorRef();

	//공격Action 사이의 Interval을 관리하는 타이머를 해제
	UFUNCTION()
		void ResetAtkIntervalTimer();

// ------ 캐릭터 버프 / 디버프 ---------------
public:
	//버프와 디버프를 건다
	UFUNCTION(BlueprintCallable)
		void SetBuffTimer(bool bIsDebuff, uint8 BuffType, AActor* Causer, float TotalTime = 1.0f, float Variable = 0.0f);

	//버프 or 디버프 상태를 초기화한다
	UFUNCTION()
		void ResetStatBuffState(bool bIsDebuff, uint8 BuffType, float ResetVal);

	//특정 Debuff의 타이머를 해제한다.
	UFUNCTION()
		void ClearBuffTimer(bool bIsDebuff, uint8 BuffType);
	
	UFUNCTION()
		void ClearAllBuffTimer(bool bIsDebuff);

	//디버프가 활성화 되어있는지 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetDebuffIsActive(ECharacterDebuffType DebuffType);

	//버프가 활성화 되어있는지 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetBuffIsActive(ECharacterBuffType BuffType);

// ----- 캐릭터 애니메이션 -------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		TArray<UAnimMontage*> AttackAnimMontageArray;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* HitAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* DieAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* RollAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Animation")
		class UAnimMontage* ReloadAnimMontage;

// ------ 그 외 캐릭터 프로퍼티 / 함수 ---------------
protected:
	//UFUNCTION()
		virtual void ClearAllTimerHandle();

	//캐릭터의 스탯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStatStruct CharacterStat;

	//캐릭터의 현재 상태
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Gameplay")
		FCharacterStateStruct CharacterState;

	UPROPERTY()
		FTimerHandle DelayHandle;

	//공격 간 딜레이 핸들
	UPROPERTY()
		FTimerHandle AtkIntervalHandle;

	UPROPERTY()
		FTimerHandle ReloadTimerHandle;
};
