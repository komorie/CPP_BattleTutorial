// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCharacterWidget.h"
#include "BTCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UBTCharacterWidget::BindCharacterStat(UBTCharacterStatComponent* NewCharacterStat)
{
	CurrentCharacterStat = NewCharacterStat;

	NewCharacterStat->OnHPChanged.AddUObject(this, &UBTCharacterWidget::UpdateHPWidget);
}

void UBTCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UBTCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
