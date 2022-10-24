// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPatrolPos.generated.h"

/**
 * 
 */
UCLASS()
class CPP_BATTLETUTORIAL_API UBTTask_FindPatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTTask_FindPatrolPos();

	//태스크가 실행될 때 실행되는 함수. 실행 결과를 반환해야 함
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
