// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class CPP_BATTLETUTORIAL_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	public:
		UBTDecorator_IsInAttackRange();

	protected:
		//조건 달성에 따라 참/거짓 반환
		virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
};
