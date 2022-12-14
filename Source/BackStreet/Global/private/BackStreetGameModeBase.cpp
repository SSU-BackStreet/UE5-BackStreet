// Copyright Epic Games, Inc. All Rights Reserved.

#include "../public/BackStreetGameModeBase.h"
#include "../../StageSystem/public/Grid.h"
#include "../../StageSystem/public/Tile.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ABackStreetGameModeBase::ABackStreetGameModeBase()
{


}

void ABackStreetGameModeBase::InitGame()
{
	RemainChapter = 2;
	InitChapter();

}

void ABackStreetGameModeBase::InitChapter()
{
	if (IsValid(Chapter))
		Chapter->RemoveChapter();

	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = FVector::ZeroVector;

	Chapter = GetWorld()->SpawnActor<AGrid>(AGrid::StaticClass(), spawnLocation, rotator, spawnParams);
	Chapter->CreateMaze(3, 3);

	CurrTile = Chapter->GetCurrentTile();
	RemainChapter--;
}

void ABackStreetGameModeBase::MoveTile(uint8 NextDir)
{
	Chapter->MoveCurrentTile(NextDir);
	CurrTile = Chapter->GetCurrentTile();
	//UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::CalculateLoc()] CurrentTransform : %s"), *Map->GetCurrentTile()->GetActorLocation().ToString());
}

void ABackStreetGameModeBase::NextStage(uint8 Dir)
{
	switch ((EDirection)Dir)
	{
	case EDirection::E_UP:
		PreDir = (uint8)(EDirection::E_DOWN);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : DOWN"));
		break;
	case EDirection::E_DOWN:
		PreDir = (uint8)(EDirection::E_UP);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : UP"));
		break;
	case EDirection::E_LEFT:
		PreDir = (uint8)(EDirection::E_RIGHT);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : RIGHT"));
		break;
	case EDirection::E_RIGHT:
		PreDir = (uint8)(EDirection::E_LEFT);
		UE_LOG(LogTemp, Log, TEXT("[AtestGameMode::NextStage()] PreDir : LEFT"));
		break;
	default:
		break;
	}
	MoveTile(Dir);
}

void ABackStreetGameModeBase::PlayCameraShakeEffect(ECameraShakeType EffectType, FVector Location, float Radius)
{
	if (CameraShakeEffectList.Num() < (uint8)EffectType) return;

	Location = Location + FVector(-700.0f, 0.0f, 1212.0f); //???????? Camera?? ?????? ???? ????
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), CameraShakeEffectList[(uint8)EffectType], Location, Radius * 0.75f, Radius);
}
