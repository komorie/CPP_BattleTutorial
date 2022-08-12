// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCharacter.h"
#include "BTAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"



// Sets default values
ABTCharacter::ABTCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BTCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	SetControlMode(EControlMode::Quarter);

	GetCharacterMovement()->JumpZVelocity = 600.f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 200.0f;
	AttackRadius = 50.0f;
}

// Called when the game starts or when spawned
void ABTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ABTCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ABTCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABTCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABTCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), IE_Pressed , this, &ABTCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABTCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ABTCharacter::Attack);

}

void ABTCharacter::UpDown(float value)
{
	switch (CurrentControlMode) {
	case EControlMode::Shoulder:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), value);
		break;
	case EControlMode::Quarter:
		AddMovementInput(FVector::ForwardVector, value);
		break;
	}
}

void ABTCharacter::LeftRight(float value)
{
	switch (CurrentControlMode) {
	case EControlMode::Shoulder:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), value);
		break;
	case EControlMode::Quarter:
		AddMovementInput(FVector::RightVector, value);
		break;
	}
}

void ABTCharacter::LookUp(float value)
{
	if (CurrentControlMode == EControlMode::Shoulder) {
		AddControllerPitchInput(value);
	}
}

void ABTCharacter::Turn(float value)
{
	if (CurrentControlMode == EControlMode::Shoulder) {
		AddControllerYawInput(value);
	}
}

void ABTCharacter::ViewChange()
{
	if (CurrentControlMode == EControlMode::Shoulder) {
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::Quarter);
	}
	else if (CurrentControlMode == EControlMode::Quarter) {
		SetControlMode(EControlMode::Shoulder);
	}
}

void ABTCharacter::Jump()
{
	Super::Jump();
}

void ABTCharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		BTAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

void ABTCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	#if ENABLE_DRAW_DEBUG
		FVector TraceVec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		float HalfHeight = AttackRange * 0.5 + AttackRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}
}

void ABTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BTAnim = Cast<UBTAnimInstance>(GetMesh()->GetAnimInstance());

	BTAnim->OnMontageEnded.AddDynamic(this, &ABTCharacter::OnAttackMontageEnded);
	//FString SectionName = BTAnim->Montage_GetCurrentSection(BTAnim->WarriorAnimMontage).ToString();
	//UE_LOG(LogTemp, Log, TEXT("Section: %s"), *SectionName);
	BTAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			UE_LOG(LogTemp, Log, TEXT("OnNextAttackCheck"));
			AttackStartComboState();
			BTAnim->JumpToAttackMontageSection(CurrentCombo);
			FString SectionName = BTAnim->Montage_GetCurrentSection(BTAnim->WarriorAnimMontage).ToString();
			UE_LOG(LogTemp, Log, TEXT("Section: %s"), *SectionName);
		}
	});
	BTAnim->OnAttackHitCheck.AddUObject(this, &ABTCharacter::AttackCheck);
}

float ABTCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Log, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	return FinalDamage;
}

void ABTCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;


	switch (CurrentControlMode)
	{
		//3인칭 카메라 이동
		case EControlMode::Shoulder:
		{
			SpringArm->TargetArmLength = 450.0f;
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true;
			SpringArm->bDoCollisionTest = true;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
		}
		break;
		case EControlMode::Quarter:
		{
			SpringArm->TargetArmLength = 800.0f;
			SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
			SpringArm->bUsePawnControlRotation = false;
			SpringArm->bInheritPitch = false;
			SpringArm->bInheritRoll = false;
			SpringArm->bInheritYaw = false;
			SpringArm->bDoCollisionTest = false;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
		}
		break;
	}
}

void ABTCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
}

void ABTCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = CurrentCombo + 1;
	UE_LOG(LogTemp, Log, TEXT("CurrentCombo: %d"), CurrentCombo);

}

void ABTCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}


