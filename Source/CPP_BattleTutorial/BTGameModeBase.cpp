// Fill out your copyright notice in the Description page of Project Settings.


#include "BTGameModeBase.h"
#include "BTPlayerController.h"
#include "BTCharacter.h"

ABTGameModeBase::ABTGameModeBase()
{
	DefaultPawnClass = ABTCharacter::StaticClass();
	PlayerControllerClass = ABTPlayerController::StaticClass();
    static ConstructorHelpers::FClassFinder<APawn> Player(TEXT("Blueprint'/Game/ThirdPersonBP/Blueprints/BP_BTCharacter.BP_BTCharacter'"));
    if (Player.Succeeded())
    {
        DefaultPawnClass = Player.Class;
    }
}
