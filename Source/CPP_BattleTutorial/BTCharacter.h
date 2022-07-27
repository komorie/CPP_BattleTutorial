// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BTCharacter.generated.h"

UCLASS()
class CPP_BATTLETUTORIAL_API ABTCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* Camera;



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

	void MoveUp();

	void MoveRight();

};
