// Fill out your copyright notice in the Description page of Project Settings.


#include "BTCharacter.h"
#include "BTAnimInstance.h"
#include "BTWeapon.h"
#include "BTCharacterStatComponent.h"
#include "BTCharacterWidget.h"
#include "BTAIController.h"
#include "BTCharacterSetting.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/AssetManager.h"



// Sets default values
ABTCharacter::ABTCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BTCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//순서에 주의. 캐릭터의 skeletalmesh를 먼저 지정하고, 소켓을 찾아야 한다.
	/*FName WeaponSocket(TEXT("hand_rSocket"));
	UE_LOG(LogTemp, Log, TEXT("%s"), GetMesh()->DoesSocketExist(WeaponSocket) ? TEXT("true") : TEXT("false"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
		if (SK_WEAPON.Succeeded())
		{
			Weapon->SetSkeletalMesh(SK_WEAPON.Object);
		}
		UE_LOG(LogTemp, Log, TEXT("%s"), GetMesh()->DoesSocketExist(WeaponSocket) ? TEXT("true") : TEXT("false"));
		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}*/

	CharacterStat = CreateDefaultSubobject<UBTCharacterStatComponent>(TEXT("CHARACTERSTAT"));

	SetControlMode(EControlMode::Quarter);

	GetCharacterMovement()->JumpZVelocity = 600.f;

	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	/*캐릭터의 AI 컨트롤러 클래스를 BTAIController 클래스로 설정.
	배치되거나 생성되는 캐릭터마다 자동으로 BTAIController가 생성 후 빙의.*/
	AIControllerClass = ABTAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	////캐릭터 에셋 정보가 저장된 디폴트 생성 클래스에서 가져오기
	//auto DefaultSetting = GetDefault<UBTCharacterSetting>();
	//if (DefaultSetting->CharacterAssets.Num() > 0)
	//{
	//	for (auto CharacterAsset : DefaultSetting->CharacterAssets)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
	//	}
	//}




}

// Called when the game starts or when spawned
void ABTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsPlayerControlled())
	{
		//NPC 캐릭터일 때, 캐릭터 에셋 정보가 저장된 디폴트 생성 클래스에서 랜덤으로 에셋 가져오기
		auto DefaultSetting = GetDefault<UBTCharacterSetting>();
		int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];
		UE_LOG(LogTemp, Log, TEXT("%s"), *CharacterAssetToLoad.ToString());
		AssetStreamingHandle = UAssetManager::GetStreamableManager()
			.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ABTCharacter::OnAssetLoadCompleted));
	}


	auto CharacterWidget = Cast<UBTCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}

	//주의!! GetWorld 함수의 경우 캐릭터가 스폰된 월드를 리턴하는데, 
	//null을 리턴해서 오류가 났다. 
	//보니까 생성자에서 사용해서 아직 액터가 스폰이 안되서 그런듯.
	// PostInitializeComponents에서 함수를 사용하는 걸로 해결
	//FName WeaponSocket(TEXT("hand_rSocket"));
	//auto CurWeapon = GetWorld()->SpawnActor<ABTWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (CurWeapon)
	//{
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	//}
}

// Called every frame
void ABTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ABTCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ABTCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABTCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABTCharacter::Turn);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), IE_Pressed , this, &ABTCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ABTCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ABTCharacter::Attack);

}

void ABTCharacter::UpDown(float value)
{
	switch (CurrentControlMode) {
	case EControlMode::Shoulder:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), value);
		break;
	case EControlMode::Quarter:
		AddMovementInput(FVector::ForwardVector, value);
		break;
	}
}

void ABTCharacter::LeftRight(float value)
{
	switch (CurrentControlMode) {
	case EControlMode::Shoulder:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), value);
		break;
	case EControlMode::Quarter:
		AddMovementInput(FVector::RightVector, value);
		break;
	}
}

void ABTCharacter::LookUp(float value)
{
	if (CurrentControlMode == EControlMode::Shoulder) {
		AddControllerPitchInput(value);
	}
}

void ABTCharacter::Turn(float value)
{
	if (CurrentControlMode == EControlMode::Shoulder) {
		AddControllerYawInput(value);
	}
}

void ABTCharacter::ViewChange()
{
	if (CurrentControlMode == EControlMode::Shoulder) {
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::Quarter);
	}
	else if (CurrentControlMode == EControlMode::Quarter) {
		SetControlMode(EControlMode::Shoulder);
	}
}

void ABTCharacter::Jump()
{
	Super::Jump();
}

void ABTCharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		BTAnim->PlayAttackMontage();
		IsAttacking = true;
	}
}

void ABTCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	#if ENABLE_DRAW_DEBUG
		FVector TraceVec = GetActorForwardVector() * AttackRange;
		FVector Center = GetActorLocation() + TraceVec * 0.5f;
		float HalfHeight = AttackRange * 0.5 + AttackRadius;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.0f;

		DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
	#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);

		}
	}
}

void ABTCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	if (nullptr != AssetLoaded)
	{
		GetMesh()->SetSkeletalMesh(AssetLoaded);
	}
}

bool ABTCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void ABTCharacter::SetWeapon(ABTWeapon* NewWeapon)
{
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void ABTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BTAnim = Cast<UBTAnimInstance>(GetMesh()->GetAnimInstance());

	BTAnim->OnMontageEnded.AddDynamic(this, &ABTCharacter::OnAttackMontageEnded);
	//FString SectionName = BTAnim->Montage_GetCurrentSection(BTAnim->WarriorAnimMontage).ToString();
	//UE_LOG(LogTemp, Log, TEXT("Section: %s"), *SectionName);
	BTAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;
		if (IsComboInputOn)
		{
			//UE_LOG(LogTemp, Log, TEXT("OnNextAttackCheck"));
			AttackStartComboState();
			BTAnim->JumpToAttackMontageSection(CurrentCombo);
			FString SectionName = BTAnim->Montage_GetCurrentSection(BTAnim->WarriorAnimMontage).ToString();
			//UE_LOG(LogTemp, Log, TEXT("Section: %s"), *SectionName);
		}
	});
	BTAnim->OnAttackHitCheck.AddUObject(this, &ABTCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		BTAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	});
}

float ABTCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Log, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	

	
	CharacterStat->SetDamage(FinalDamage);

	return FinalDamage;
}

void ABTCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;


	switch (CurrentControlMode)
	{
		//3인칭 카메라 이동
		case EControlMode::Shoulder:
		{
			SpringArm->TargetArmLength = 450.0f;
			SpringArm->bUsePawnControlRotation = true;
			SpringArm->bInheritPitch = true;
			SpringArm->bInheritRoll = true;
			SpringArm->bInheritYaw = true;
			SpringArm->bDoCollisionTest = true;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
		}
		break;
		case EControlMode::Quarter:
		{
			SpringArm->TargetArmLength = 800.0f;
			SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
			SpringArm->bUsePawnControlRotation = false;
			SpringArm->bInheritPitch = false;
			SpringArm->bInheritRoll = false;
			SpringArm->bInheritYaw = false;
			SpringArm->bDoCollisionTest = false;
			bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
		}
		break;
	}
}

void ABTCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void ABTCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = CurrentCombo + 1;
	UE_LOG(LogTemp, Log, TEXT("CurrentCombo: %d"), CurrentCombo);

}

void ABTCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}


