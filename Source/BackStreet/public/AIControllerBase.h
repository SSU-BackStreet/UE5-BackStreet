// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BackStreet.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class BACKSTREET_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION()
		virtual void OnPossess(APawn* PossessedPawn) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateTargetPerception(AActor* Actor, FAIStimulus Stimulus);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;

};
