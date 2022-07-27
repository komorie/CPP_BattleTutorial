// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABTCharacter::ABTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 100.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerModel(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (PlayerModel.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(PlayerModel.Object);
	}
	
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

}

void ABTCharacter::MoveUp()
{
}

void ABTCharacter::MoveRight()
{
}

