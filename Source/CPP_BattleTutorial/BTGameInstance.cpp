// Fill out your copyright notice in the Description page of Project Settings.


#include "BTGameInstance.h"

UBTGameInstance::UBTGameInstance()
{
    FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
    BTCharacterTable = DT_ABCHARACTER.Object;

    //if (BTCharacterTable->GetRowMap().Num() > 0)
    //{

    //}
}

void UBTGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Log, TEXT("Inita"));
    UE_LOG(LogTemp, Log, TEXT("%d"), GetBTCharacterData(20)->DropExp);
}



FBTCharacterData* UBTGameInstance::GetBTCharacterData(int32 Level)
{
    return BTCharacterTable->FindRow<FBTCharacterData>(*FString::FromInt(Level), TEXT(""));
}
