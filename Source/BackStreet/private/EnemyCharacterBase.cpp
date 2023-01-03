// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/EnemyCharacterBase.h"
#include "../public/WeaponBase.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	this->Tags.Add("Enemy");
}

void AEnemyCharacterBase::TryAttack()
{
	Super::TryAttack();
}

void AEnemyCharacterBase::Attack()
{
	Super::Attack();
}

void AEnemyCharacterBase::StopAttack()
{
	Super::StopAttack();
}
