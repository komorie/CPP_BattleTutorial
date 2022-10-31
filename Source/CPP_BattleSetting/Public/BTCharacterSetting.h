// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BTCharacterSetting.generated.h"

/**
 * 
 */



//ini���� ���� ��� ��������
UCLASS(config = Character)

class CPP_BATTLESETTING_API UBTCharacterSetting : public UObject
{
	GENERATED_BODY()


public:
	UBTCharacterSetting();

	//���� ��� ������ �����ϴ� Ŭ����
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
	
};
