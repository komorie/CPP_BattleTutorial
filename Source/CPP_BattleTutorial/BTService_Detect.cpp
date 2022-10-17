// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "BTAIController.h"
#include "BTCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	UE_LOG(LogTemp, Log, TEXT("DETECT"));
	//TickNode ȣ�� ����
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//�����̺�� Ʈ���� ���� AI�� ���� �����ϴ� �� ��������
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	//���� ���� ����, ���� ��ġ �������� Ž�� ���� ����.
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World) return;


	//*���� ���� ��ġ�� �������� Ž�� ������ŭ�� ������ ���� �� �ȿ� �浹�ϴ� ���
	// �� �ִ��� �Ǻ��ϰ� �浹 ����(bResult), �浹�� ����/������Ʈ(OverlapResults)
	//����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			//�浹�� ��� ���͸� ĳ���ͷ� ĳ����
			ABTCharacter* BTCharacter = Cast<ABTCharacter>(OverlapResult.GetActor());
			
			//���Ͱ� ĳ���Ͱ� �´���, �׸��� �÷��̾ �����ϴ� ĳ�������� �Ǻ�
			if (BTCharacter && BTCharacter->GetController()->IsPlayerController())
			{
				//������ �������� Target Ű�� �� ĳ���ͷ� ������ ����� ������ �׸���
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABTAIController::TargetKey, BTCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, BTCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), BTCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	//�浹 ����� ������ Target Ƽ�� nullptr��
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABTAIController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
