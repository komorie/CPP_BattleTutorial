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

	//AI가 조종하는 폰을 가져옴(없으면 거짓)
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	//블랙보드의 타겟 키 받아옴(타겟을 찾았는지 판별)
	auto Target = Cast<ABTCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABTAIController::TargetKey));
	if (nullptr == Target)
		return false;

	//타겟 캐릭터가 공격가능한 거리 내에 있는지 판별
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);
	return bResult;
}
