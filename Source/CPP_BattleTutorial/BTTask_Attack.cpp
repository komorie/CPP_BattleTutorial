// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BTAIController.h"
#include "BTCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//비헤이비어 트리를 가진 AI가 현재 조종하는 폰을 가져와 캐릭터로 캐스팅
	auto BTCharacter = Cast<ABTCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BTCharacter)
		return EBTNodeResult::Failed;

	//캐릭터의 공격 함수 실행. 상태를 공격 중으로 바꿈.
	BTCharacter->Attack();
	IsAttacking = true;

	//델리게이트를 이용해 캐릭터의 공격 몽타주가 끝나면 상태를 false로 바꿈.
	BTCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	//태스크의 수행 결과는 아직 진행 중. 공격 몽타주가 끝날 때 성공을 반환해야 함.
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 공격 상태를 틱마다 살펴보다가 공격이 끝나면 성공을 반환한다.
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
