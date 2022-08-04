// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAnimInstance.h"
#include "BTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTAnimInstance::UBTAnimInstance()
{
	CurrentPawnSpeed = 0.f;
}

void UBTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();

	if (IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		if (Pawn)
		{
			auto PlayerCharacter = Cast<ACharacter>(Pawn);
			IsFalling = PlayerCharacter->GetMovementComponent()->IsFalling();
		}
	}
}
