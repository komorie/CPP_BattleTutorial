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
	
	//�����̺�� Ʈ���� ���� AI�� ���� �����ϴ� ���� ������ ĳ���ͷ� ĳ����
	auto BTCharacter = Cast<ABTCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BTCharacter)
		return EBTNodeResult::Failed;

	//AI�� �����ϴ� ���� �������� Target(�÷��̾� ĳ����) Ű�� ������.
	auto Target = Cast<ABTCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABTAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	//AI �������� Ÿ���� �ٶ󺸴� ���� ���ϱ�
	FVector LookVector = Target->GetActorLocation() - BTCharacter->GetActorLocation();
	LookVector.Z = 0.0f;

	//*�ϴ� AI ĳ���͸� ȸ����Ŵ
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	BTCharacter->SetActorRotation(FMath::RInterpTo(BTCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
