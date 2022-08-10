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
		WarriorAnimMontage = ATTACK_MONTAGE.Object;
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
	Montage_Play(WarriorAnimMontage, 1.f);
}

void UBTAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	//���⼭ Attack2�� ������ �ȵȴ�.
	//�ƴ� �ذ��� �ƴ�.. Attack2�� �����ϱ� ����.
	//�� NextAttackCheck��Ƽ���̰� �� �Լ��� ȣ���ϴµ�
	//��Ƽ���̰� ȣ��ǰ� ��Ÿ�ְ� ���������� Ÿ�̹��� ª�Ƽ� ���� �Լ��� ȣ��Ǳ�
	//���� ��Ÿ�ְ� ���������� ������ ����... �׷��� ���� ��Ÿ�ָ� ���ϱ� None�� ������ �ſ���
	//NextAttackCheck ��Ƽ���� �ð��� ���ܼ� �ذ᤻��

	//�ٵ� �׷��� �޺��� �ֱ� ���ؼ� �ʿ�� �ϴ� Ŭ�� �ð��� ����������.
	//�׷��� ��Ƽ���� �ð��� �ΰ� �ִϸ��̼��� ������ �ð��� �÷�����. ��

	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), WarriorAnimMontage);
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
	UE_LOG(LogTemp, Log, TEXT("Attack%d"), Section);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
