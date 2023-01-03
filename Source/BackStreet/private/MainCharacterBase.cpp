// Fill out your copyright notice in the Description page of Project Settings.


#include "../public/MainCharacterBase.h"
#include "../public/WeaponBase.h"
#include "../public/MainCharacterController.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

// Sets default values
AMainCharacterBase::AMainCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CAMERA_BOOM"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 1400.0f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->SetRelativeRotation({ -60.0f, 0.0f, 0.0f });

	FollowingCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FOLLOWING_CAMERA"));
	FollowingCamera->SetupAttachment(CameraBoom);
	FollowingCamera->bAutoActivate = false;

	this->bUseControllerRotationYaw = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->RotationRate = { 0.0f, 0.0f, 750.0f };
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->GravityScale = 2.5f;

	this->Tags.Add("Player");
}

// Called when the game starts or when spawned
void AMainCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerControllerRef = Cast<AMainCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called every frame
void AMainCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacterBase::MoveRight);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharacterBase::Dash);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMainCharacterBase::Roll);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AMainCharacterBase::TryAttack);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AMainCharacterBase::TryReload);
}

void AMainCharacterBase::MoveForward(float Value)
{
	FVector Direction = FVector(1.0f, 0.0f, 0.0f);// FRotationMatrix(this->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::MoveRight(float Value)
{
	FVector Direction = FVector(0.0f, 1.0f, 0.0f);//FRotationMatrix(this->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMainCharacterBase::Dash()
{
	if (!IsValid(RollAnimMontage) || !GetIsActionActive(ECharacterActionType::E_Idle)) return;
	
	FVector newDirection(0.0f);
	newDirection.X = GetInputAxisValue(FName("MoveForward"));
	newDirection.Y = GetInputAxisValue(FName("MoveRight"));

	FRotator newRotation = { 0, FMath::Atan2(newDirection.Y, newDirection.X) * 180.0f / 3.141592, 0.0f};
	GetCapsuleComponent()->SetWorldRotation(newRotation);

	CharacterState.CharacterActionState = ECharacterActionType::E_Roll;
	PlayAnimMontage(RollAnimMontage);

	GetWorld()->GetTimerManager().SetTimer(DelayHandle, FTimerDelegate::CreateLambda([&]() {
		ResetActionState();
	}), 0.5f, false);
}

void AMainCharacterBase::TryReload()
{
	Super::TryReload();
}

void AMainCharacterBase::TryAttack()
{
	Super::TryAttack();
	
	if (CharacterState.CharacterActionState == ECharacterActionType::E_Idle
		|| CharacterState.CharacterActionState == ECharacterActionType::E_Attack)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCapsuleComponent()->SetWorldRotation(PlayerControllerRef->GetAimingRotation());
		GetWorld()->GetTimerManager().ClearTimer(RotationFixTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RotationFixTimerHandle, FTimerDelegate::CreateLambda([&]() {
			GetCharacterMovement()->bOrientRotationToMovement = true;
			}), 1.0f, false);
	}
}

void AMainCharacterBase::Attack()
{
	Super::Attack();
}


void AMainCharacterBase::StopAttack()
{
	Super::StopAttack();
	if (IsValid(WeaponActor->GetChildActor()))
	{
		AWeaponBase* weaponRef = Cast<AWeaponBase>(WeaponActor->GetChildActor());
		weaponRef->StopAttack();
	}
}

void AMainCharacterBase::ClearAllTimerHandle()
{
	Super::ClearAllTimerHandle();

	GetWorld()->GetTimerManager().ClearTimer(RotationFixTimerHandle);
}
