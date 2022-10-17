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
	//TickNode 호출 간격
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//비헤이비어 트리를 가진 AI가 현재 조종하는 폰 가져오기
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	//폰이 속한 세계, 현재 위치 가져오고 탐지 범위 설정.
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World) return;


	//*폰의 현재 위치를 기준으로 탐지 범위만큼의 지름을 가진 구 안에 충돌하는 대상
	// 이 있는지 판별하고 충돌 여부(bResult), 충돌한 액터/컴포넌트(OverlapResults)
	//저장
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
			//충돌한 대상 액터를 캐릭터로 캐스팅
			ABTCharacter* BTCharacter = Cast<ABTCharacter>(OverlapResult.GetActor());
			
			//액터가 캐릭터가 맞는지, 그리고 플레이어가 조작하는 캐릭터인지 판별
			if (BTCharacter && BTCharacter->GetController()->IsPlayerController())
			{
				//맞으면 블랙보드의 Target 키를 그 캐릭터로 지정후 디버그 도형을 그린다
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABTAIController::TargetKey, BTCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, BTCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), BTCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	//충돌 대상이 없으면 Target 티를 nullptr로
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABTAIController::TargetKey, nullptr);
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
