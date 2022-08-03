// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAnimInstance.h"

UBTAnimInstance::UBTAnimInstance()
{
	CurrentPawnSpeed = 0.f;
}

void UBTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto PlayerCharacter = TryGetPawnOwner();

	if (IsValid(PlayerCharacter))
	{
		CurrentPawnSpeed = PlayerCharacter->GetVelocity().Size();
	}
}
