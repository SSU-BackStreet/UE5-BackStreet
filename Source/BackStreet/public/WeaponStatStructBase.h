#pragma once

#include "Engine/DataTable.h"
#include "CharacterInfoStructBase.h"
#include "WeaponStatStructBase.generated.h"

USTRUCT(BlueprintType)
struct FProjectileStatStruct
{
public:
	GENERATED_USTRUCT_BODY()

	//발사체의 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileSpeed = 2000.0f;

	//발사체의 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float ProjectileDamage = 0.2f;

	//중력 스케일
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float GravityScale = 1.0f;

	//유도가 되는지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsHoming = false;

	//발사체는 각 하나의 디버프만 가짐
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;
};


USTRUCT(BlueprintType)
struct FWeaponStatStruct
{
public:
	GENERATED_USTRUCT_BODY()
	
	
	//----- 근접 관련 Property --------------------

	//근접 공격이 가능한 지? 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bCanMeleeAtk = true;

	//공격 속도 Rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponAtkSpeedRate = 1.0f;

	//무기 데미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float WeaponDamage = 0.2f;

	//무기는 각 하나의 디버프만 가짐 (임시)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		ECharacterDebuffType DebuffType;


	//----- 발사체 관련 Property ------------------
	
	//무한 탄약인지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bIsInfiniteAmmo;

	//공격 시, 발사체가 있는지?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bHasProjectile;

	//탄창이 필요없는지? (ex. 기본 투사체 등)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		bool bInfiniteMagazine;

	//한 탄창에 최대 발사체 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 MaxAmmoPerMagazine; 

	//장전 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float LoadingDelayTime;

	//발사체 스탯
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FProjectileStatStruct ProjectileStat;

};