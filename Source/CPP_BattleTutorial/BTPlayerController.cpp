// Fill out your copyright notice in the Description page of Project Settings.


#include "BTPlayerController.h"

void ABTPlayerController::BeginPlay()
{
    Super::BeginPlay();

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}
