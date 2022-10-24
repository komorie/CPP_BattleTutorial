// Fill out your copyright notice in the Description page of Project Settings.



#include "BTCharacterStatComponent.h"
#include "BTGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBTCharacterStatComponent::UBTCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	Level = 1;
	// ...
}


// Called when the game starts
void UBTCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBTCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}


// Called every frame
void UBTCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBTCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto BTGameInstance = Cast<UBTGameInstance>(GetWorld()->GetGameInstance());

	if (BTGameInstance != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ye"));
		CurrentStatData = BTGameInstance->GetBTCharacterData(NewLevel);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("fuck"));
	}
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void UBTCharacterStatComponent::SetDamage(float NewDamage)
{
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UBTCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UBTCharacterStatComponent::GetAttack()
{
	return CurrentStatData->Attack;
}

float UBTCharacterStatComponent::GetHPRatio()
{
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

