// Fill out your copyright notice in the Description page of Project Settings.


#include "BTPawn.h"


// Sets default values
ABTPawn::ABTPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	

}

// Called when the game starts or when spawned
void ABTPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABTPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABTPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

