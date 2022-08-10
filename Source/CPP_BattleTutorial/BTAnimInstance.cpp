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
	//여기서 Attack2로 점프가 안된다.
	//아니 해결이 됐다.. Attack2로 점프하기 전에.
	//즉 NextAttackCheck노티파이가 이 함수를 호출하는데
	//노티파이가 호출되고 몽타주가 끝날때까지 타이밍이 짧아서 점프 함수가 호출되기
	//전에 몽타주가 끝나버려서 점프를 못함... 그래서 현재 몽타주를 보니까 None이 나오는 거였네
	//NextAttackCheck 노티파이 시간을 땡겨서 해결ㅋㅋ

	//근데 그러면 콤보를 넣기 위해서 필요로 하는 클릭 시간이 빡빡해진다.
	//그래서 노티파이 시간은 두고 애니메이션이 끝나는 시간을 늘려버림. 끝

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
