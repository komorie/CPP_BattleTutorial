// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAnimInstance.h"
#include "BTCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTAnimInstance::UBTAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsFalling = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage>ATTACK_MONTAGE(TEXT("AnimMontage'/Game/Book/Animations/WarriorAnimMontage.WarriorAnimMontage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

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

void UBTAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UBTAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UBTAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UBTAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UBTAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
