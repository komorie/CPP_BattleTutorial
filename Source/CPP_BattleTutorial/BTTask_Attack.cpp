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
	
	//�����̺�� Ʈ���� ���� AI�� ���� �����ϴ� ���� ������ ĳ���ͷ� ĳ����
	auto BTCharacter = Cast<ABTCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BTCharacter)
		return EBTNodeResult::Failed;

	//ĳ������ ���� �Լ� ����. ���¸� ���� ������ �ٲ�.
	BTCharacter->Attack();
	IsAttacking = true;

	//��������Ʈ�� �̿��� ĳ������ ���� ��Ÿ�ְ� ������ ���¸� false�� �ٲ�.
	BTCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	//�½�ũ�� ���� ����� ���� ���� ��. ���� ��Ÿ�ְ� ���� �� ������ ��ȯ�ؾ� ��.
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� ���¸� ƽ���� ���캸�ٰ� ������ ������ ������ ��ȯ�Ѵ�.
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
