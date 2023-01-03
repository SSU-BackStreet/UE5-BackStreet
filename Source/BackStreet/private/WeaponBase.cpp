// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "CharacterBase.h"
#include "../public/ProjectileBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DEFAULT_SCENE_ROOT"));
	DefaultSceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(DefaultSceneRoot);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON_MESH"));
	WeaponMesh->SetupAttachment(DefaultSceneRoot);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Attack()
{
	//발사체가 있는 무기라면 발사
	if (WeaponStat.bHasProjectile) 
	{
		TryFireProjectile();
	}
	//근접 공격이 가능한 무기라면 근접 공격 로직 수행
	if (WeaponStat.bCanMeleeAtk)
	{
		GetWorldTimerManager().SetTimer(MeleeAtkTimerHandle, this, &AWeaponBase::MeleeAttack, 0.01f, true);
		GetWorldTimerManager().SetTimer(MeleeComboTimerHandle, this, &AWeaponBase::ResetCombo, 1.0f, false, 1.0f);
		ComboCnt = (ComboCnt + 1);
	}
}

void AWeaponBase::StopAttack()
{
	GetWorldTimerManager().ClearTimer(MeleeAtkTimerHandle);
	MeleeLineTraceQueryParams.ClearIgnoredActors();
	MeleeLineTraceQueryParams.AddIgnoredActor(OwnerCharacterRef);
}

void AWeaponBase::InitWeaponStat(FWeaponStatStruct NewStat)
{
	WeaponStat = NewStat;
}

AProjectileBase* AWeaponBase::CreateProjectile()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; //Projectile의 소유자는 Player
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator SpawnRotation = OwnerCharacterRef->GetActorRotation();
	FVector SpawnLocation = OwnerCharacterRef->GetActorLocation();
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetActorForwardVector() * 100.0f;
	SpawnLocation = SpawnLocation + OwnerCharacterRef->GetActorRightVector() * 25.0f;
	FTransform SpawnTransform = { SpawnRotation, SpawnLocation, {1.0f, 1.0f, 1.0f} };

	AProjectileBase* newProjectile = Cast<AProjectileBase>(GetWorld()->SpawnActor(ProjectileClass, &SpawnTransform, SpawnParams));

	if (IsValid(newProjectile))
	{
		newProjectile->InitProjectile(WeaponStat.ProjectileStat, OwnerCharacterRef);
		return newProjectile;
	}

	return nullptr;
}

bool AWeaponBase::TryReload()
{
	if (!GetCanReload()) return false;

	int32 addAmmoCnt = FMath::Min(MaxAmmoCount, WeaponStat.MaxAmmoPerMagazine);
	if (addAmmoCnt + CurrentAmmoCount > WeaponStat.MaxAmmoPerMagazine)
		addAmmoCnt = (WeaponStat.MaxAmmoPerMagazine - CurrentAmmoCount);

	CurrentAmmoCount += addAmmoCnt; 
	MaxAmmoCount -= addAmmoCnt;

	return true;
}

bool AWeaponBase::GetCanReload()
{
	if (WeaponStat.bIsInfiniteAmmo || !WeaponStat.bHasProjectile) return false;
	if (MaxAmmoCount == 0 || CurrentAmmoCount == WeaponStat.MaxAmmoPerMagazine) return false;
	return true;
}

void AWeaponBase::AddAmmo(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || MaxAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	MaxAmmoCount = (MaxAmmoCount + Count) % MAX_AMMO_LIMIT_CNT;
}

void AWeaponBase::AddMagazine(int32 Count)
{
	if (WeaponStat.bIsInfiniteAmmo || MaxAmmoCount >= MAX_AMMO_LIMIT_CNT) return;
	MaxAmmoCount = (MaxAmmoCount + WeaponStat.MaxAmmoPerMagazine * Count) % MAX_AMMO_LIMIT_CNT;
}

bool AWeaponBase::TryFireProjectile()
{
	if (CurrentAmmoCount == 0 && !WeaponStat.bIsInfiniteAmmo)
	{
		TryReload();
		return false;
	}

	AProjectileBase* newProjectile = CreateProjectile();

	//스폰한 발사체가 Valid 하다면 발사
	if (IsValid(newProjectile))
	{
		if(!WeaponStat.bIsInfiniteAmmo) CurrentAmmoCount -= 1;
		newProjectile->ActivateProjectileMovement();
		return true;
	}
	return false;
}

void AWeaponBase::MeleeAttack()
{	
	FHitResult hitResult;
	FVector StartLocation = WeaponMesh->GetSocketLocation(FName("GrabPoint"));
	FVector EndLocation = WeaponMesh->GetSocketLocation(FName("End"));

	//LineTrace를 통해 hit 된 물체들을 추적
	GetWorld()->LineTraceSingleByChannel(hitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, MeleeLineTraceQueryParams);
	
	//	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 0), false, 1.0f, 0, 1.5f);

	//hit 되었다면?
	if (hitResult.bBlockingHit && hitResult.GetActor()->ActorHasTag("Character")
		&& !hitResult.GetActor()->ActorHasTag(OwnerCharacterRef->Tags[1]))
	{
		//데미지를 주고
		UGameplayStatics::ApplyDamage(hitResult.GetActor(), WeaponStat.WeaponDamage
										, OwnerCharacterRef->GetController(), OwnerCharacterRef, nullptr);
		Cast<ACharacterBase>(hitResult.GetActor())->SetBuffTimer(true, (uint8)WeaponStat.DebuffType, OwnerCharacterRef, 1.0f, 0.02f);

		//효과 이미터 출력
		if (IsValid(HitEffectParticle))
		{
			FTransform emitterSpawnTransform(FQuat(0.0f), hitResult.Location, FVector(1.0f));
			MeleeLineTraceQueryParams.AddIgnoredActor(hitResult.GetActor());
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffectParticle, emitterSpawnTransform, true, EPSCPoolMethod::None, true);
		}
	}
}

void AWeaponBase::ResetCombo()
{
	ComboCnt = 0;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::InitOwnerCharacterRef(ACharacterBase* NewCharacterRef)
{
	if (!IsValid(NewCharacterRef)) return;
	OwnerCharacterRef = NewCharacterRef;
	MeleeLineTraceQueryParams.AddIgnoredActor(NewCharacterRef);
}

