// Fill out your copyright notice in the Description page of Project Settings.


#include "BTAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

//������ Ű �̸�
const FName ABTAIController::HomePosKey(TEXT("HomePos"));
const FName ABTAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABTAIController::TargetKey(TEXT("Target"));

ABTAIController::ABTAIController()
{
	//������, �����̺�� Ʈ�� ����
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
	if (UseBlackboard(BBAsset, Blackboard)) //AI�� Ư�� ������ ������ ������Ʈ�� ����ϵ��� ��
	{
		//HomePos ���� ���� AI�� �����̴� ���� ��ġ��
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		
		//�����̺�� Ʈ�� ����
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp, Log, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

