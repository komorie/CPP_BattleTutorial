// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "BTAIController.h"
#include "BTCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//AI�� �����ϴ� ���� ������(������ ����)
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	//�������� Ÿ�� Ű �޾ƿ�(Ÿ���� ã�Ҵ��� �Ǻ�)
	auto Target = Cast<ABTCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABTAIController::TargetKey));
	if (nullptr == Target)
		return false;

	//Ÿ�� ĳ���Ͱ� ���ݰ����� �Ÿ� ���� �ִ��� �Ǻ�
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;
}
