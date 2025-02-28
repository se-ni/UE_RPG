// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "../Global/GlobalGameInstance.h"
#include "MyAIController.h"
#include "../Global/GlobalEnums.h"
#include "MonsterData.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Stage3/MainPlayerCharacter3.h"
#include "../Global/Projectile.h"
#include "Components/AudioComponent.h"
#include "../Global/Projectile.h"

ABoss::ABoss()
{

}

void ABoss::BeginPlay()
{	
	// Super::BeginPlay();
	SetAllSound(MapSound);

	if (nullptr == AudioComponent)
	{
		// 비긴플레이에서 한번만 찾는게 가장 좋다.
		AudioComponent = Cast<UAudioComponent>(GetComponentByClass(UAudioComponent::StaticClass()));
		AudioComponent->Stop();
	}

	GlobalAnimInstance = Cast<UGlobalAnimInstance>(GetMesh()->GetAnimInstance());

	UGlobalGameInstance* GameInst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != GameInst) // 글로벌게임인스턴스를 통해서
								// 애니스테이트와 애니메이션을 세팅해준다
	{
		CurMonsterData = GameInst->GetMonsterData(DataName); // 현재 몬스터데이터에 내용 넣어주고

		SetAllAnimation(CurMonsterData->MapAnimation);
		SetAniState(EAniState::None);
	}
	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIAniState"), static_cast<uint8>(EAniState::Idle));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 1500.0f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), 2000.0f);

	GetBlackboardComponent()->SetValueAsVector(TEXT("OriginPos"), GetActorLocation());

	GetBlackboardComponent()->SetValueAsBool(TEXT("bIsDeath"), false);

	CapsuleComp = GetCapsuleComponent();
	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss::BeginOverlap);

	GetBlackboardComponent()->SetValueAsBool(TEXT("SpawnCoin"), false);

	GetBlackboardComponent()->SetValueAsFloat(TEXT("BossMonsterHP"), 4.0f); // 블랙보드 BossHP SET

	GlobalAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ABoss::AnimNotifyBegin);
}

void ABoss::Tick(float DeltaSecond)
{
	Super::Tick(DeltaSecond);
}

void ABoss::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	int a = 0;
	// 플레이어의 총알에 맞았을경우. hp 감소
	AProjectile* Projectile = Cast<AProjectile>(OtherActor);
	if (Projectile != nullptr)
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("bIsDeath"), true); // 보스몬스터 death로
	}
}
//void ABoss::EndOverlap(UPrimitiveComponent* OverlappedComponent,
//	AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,
//	int32 OtherBodyIndex)
//{
//	int a = 0;
//	GetBlackboardComponent()->SetValueAsBool(TEXT("bIsDeath"), false); // 보스몬스터 death false로
//}


void ABoss::AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	AudioComponent->SetSound(MapSound[EAniState::Attack]);
	AudioComponent->Play();

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	TSubclassOf<UObject> Fire = Inst->GetSubClass(TEXT("Fire"));

	TArray<UActorComponent*> FireEffects = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("FireEffect"));
	USceneComponent* FireCom = Cast<USceneComponent>(FireEffects[0]);

	FVector Pos = FireCom->GetComponentToWorld().GetLocation();

	if (nullptr != Fire)
	{
		{	// 발사체 만들기
			AActor* Actor = GetWorld()->SpawnActor<AActor>(Fire);
			FireActor = Cast<AFire>(Actor);

			FireActor->SetActorLocation(Pos);
			FireActor->SetActorRotation(GetActorRotation());
			FireActor->Tags.Add(FName("MonsterAttack"));
			FireActor->GetSphereComponent()->SetCollisionProfileName(TEXT("MonsterAttack"), true);
		}
	}
}

void ABoss::DestroyAttackEffect()
{
	if (AttackEffect)
	{
		AttackEffect->Destroy();
		AttackEffect = nullptr;
	}
}

void ABoss::Setbdeathfalse()
{
	GetBlackboardComponent()->SetValueAsBool(TEXT("bIsDeath"), false);
}