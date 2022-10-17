// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 * 
 */
UCLASS()
class CPP_BATTLETUTORIAL_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()
	public:
		UBTService_Detect();

	protected:
		//Ʈ������ ��尡 Ȱ��ȭ�Ǹ� �� ƽ���� ����
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
