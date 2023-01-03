// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		virtual void BeginPlay() override;

public:
	//캐릭터가 조준하고 있는 Rotation을 반환
	UFUNCTION()
		FRotator GetAimingRotation();

	//UFUNCTION()
		//FRotator GetRightAnalogRotation();

	//캐릭터가 커서의 월드 방향을 바라보는 Rotation을 반환
	UFUNCTION()
		FRotator GetRotationToCursor();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
		bool bJoyStickMode = false;

private:
	UPROPERTY()
		class AMainCharacterBase* PlayerRef;
};
