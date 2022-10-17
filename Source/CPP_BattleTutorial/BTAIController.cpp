// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

//블랙보드 키 이름
const FName ABTAIController::HomePosKey(TEXT("HomePos"));
const FName ABTAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABTAIController::TargetKey(TEXT("Target"));

ABTAIController::ABTAIController()
{
	//블랙보드, 비헤이비어 트리 참조
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/Book/AI/BB_BTCharacter.BB_BTCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/Book/AI/BT_BTCharacter.BT_BTCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void ABTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard)) //AI가 특정 블랙보드 에셋을 컴포넌트로 사용하도록 함
	{
		//HomePos 값을 현재 AI가 움직이는 폰의 위치로
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		
		//비헤이비어 트리 실행
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Log, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

