// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BTCharacter.generated.h"

UENUM()
enum class EControlMode
{
	Shoulder,
	Quarter,
	NPC
};

UCLASS()
class CPP_BATTLETUTORIAL_API ABTCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABTCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void UpDown(float value);

	void LeftRight(float value);

	void LookUp(float value);

	void Turn(float value);

protected:
	void SetControlMode(EControlMode NewControlMode);

private:
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = Camera)
	EControlMode CurrentControlMode;

};
