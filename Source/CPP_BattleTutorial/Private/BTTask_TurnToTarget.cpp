// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "BTAIController.h"
#include "BTCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	//비헤이비어 트리를 가진 AI가 현재 조종하는 폰을 가져와 캐릭터로 캐스팅
	auto BTCharacter = Cast<ABTCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BTCharacter)
		return EBTNodeResult::Failed;

	//AI가 조종하는 폰의 블랙보드의 Target(플레이어 캐릭터) 키를 가져옴.
	auto Target = Cast<ABTCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABTAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	//AI 시점에서 타겟을 바라보는 벡터 구하기
	FVector LookVector = Target->GetActorLocation() - BTCharacter->GetActorLocation();
	LookVector.Z = 0.0f;

	//*일단 AI 캐릭터를 회전시킴
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	BTCharacter->SetActorRotation(FMath::RInterpTo(BTCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
