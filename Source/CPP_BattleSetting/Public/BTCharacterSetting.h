// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BTCharacterSetting.generated.h"

/**
 * 
 */



//ini에서 에셋 경로 가져오기
UCLASS(config = Character)

class CPP_BATTLESETTING_API UBTCharacterSetting : public UObject
{
	GENERATED_BODY()


public:
	UBTCharacterSetting();

	//에셋 경로 정보를 보관하는 클래스
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
	
};
