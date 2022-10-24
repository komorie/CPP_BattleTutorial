// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BTGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FBTCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FBTCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

UCLASS()
class CPP_BATTLETUTORIAL_API UBTGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UBTGameInstance();

	virtual void Init() override;

public:
	FBTCharacterData* GetBTCharacterData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* BTCharacterTable;
};
