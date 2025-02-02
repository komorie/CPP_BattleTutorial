// Fill out your copyright notice in the Description page of Project Settings.


#include "BTGameModeBase.h"
#include "BTPlayerController.h"
#include "BTCharacter.h"

ABTGameModeBase::ABTGameModeBase()
{
	DefaultPawnClass = ABTCharacter::StaticClass();
	/*PlayerControllerClass = ABTPlayerController::StaticClass();*/
    static ConstructorHelpers::FClassFinder<ACharacter> Player(TEXT("Class'/Script/CPP_BattleTutorial.BTCharacter'"));
    if (Player.Succeeded())
    {
        DefaultPawnClass = Player.Class;
    }
}
