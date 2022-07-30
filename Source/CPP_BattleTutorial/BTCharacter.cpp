// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABTCharacter::ABTCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


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
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(0);
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

}

void ABTCharacter::UpDown(float value)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), value);
}

void ABTCharacter::LeftRight(float value)
{
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), value);
}

void ABTCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ABTCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABTCharacter::SetControlMode(int32 ControlMode)
{
	//3��Ī ī�޶� �̵�
	if (ControlMode == 0)
	{
		SpringArm->TargetArmLength = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		//SpringArm->bInheritPitch = true;
		//SpringArm->bInheritRoll = true;
		//SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);

	}
}

