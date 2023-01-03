// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterController.h"
#include "../public/MainCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void AMainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	this->bShowMouseCursor = true;

	PlayerRef = Cast<AMainCharacterBase>(GetCharacter());
}

FRotator AMainCharacterController::GetAimingRotation()
{
	//임시 코드) 추후 Attach되자마자 Gamemode를 통해 UI, Controller 등으로 Delegate Broadcast
	auto genericApplication = FSlateApplication::Get().GetPlatformApplication();
	if (genericApplication.Get() != nullptr && genericApplication->IsGamepadAttached())
	{
		//return GetRightAnalogRotation();
		return FRotator();
	}
	return GetRotationToCursor();
}

FRotator AMainCharacterController::GetRotationToCursor()
{
	FRotator retRotation;
	
	//Trace의 시작은 플레이어의 FollowingCamera 위치
	FVector traceStartLocation = PlayerRef->FollowingCamera->GetComponentLocation();
	FVector traceEndLocation, mouseDirection;
	FHitResult hitResult;

	//마우스 커서 위치에서 바닥으로의 5000만큼의 위치가 Trace의 마지막 지점
	DeprojectMousePositionToWorld(traceEndLocation, mouseDirection);
	traceEndLocation += (mouseDirection * 5000.0f);

	//카메라에서 커서의 바닥 위치까지 LineTrace를 진행 -> 실제 커서의 월드 상호작용 위치가 hitResult.Location에 담김
	GetWorld()->LineTraceSingleByChannel(hitResult, traceStartLocation, traceEndLocation
		, ECollisionChannel::ECC_Visibility);

	//hit에 성공했다면
	if (hitResult.bBlockingHit)
	{
		retRotation = UKismetMathLibrary::FindLookAtRotation(PlayerRef->GetMesh()->GetComponentLocation(), hitResult.Location);
		retRotation = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, retRotation.Yaw);
		return retRotation;
	}

	return FRotator();
}
