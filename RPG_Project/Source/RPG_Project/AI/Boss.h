// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "MonsterData.h"
#include "../Global/GlobalEnums.h"
#include "../Global/GlobalGameInstance.h"
#include "../Global/GlobalAnimInstance.h"
#include "../AI/MonsterPatrolData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Global/GlobalCoin.h"
#include "../Global/Fire.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class RPG_PROJECT_API ABoss : public AAICharacter
{
	GENERATED_BODY()

	const struct FMonsterData* CurMonsterData; // 현재 몬스터에 대한 데이터

	void BeginPlay() override;

	void Tick(float DeltaSecond) override;

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:


public:
	ABoss();

	// Declare the collision component
	UPROPERTY(VisibleAnywhere)
		UCapsuleComponent* CapsuleComp;

	//FVector GetPatrolPointLocation()
	//{
	//	return PatrolPointReference->GetActorLocation();
	//}
	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//UFUNCTION()
	//	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	//		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void SpawnCoinActor(FVector _Loc)
	{
		FTransform Trans;
		Trans.SetLocation(_Loc);
		AActor* SpawnedActor0 = GetWorld()->SpawnActor<AActor>(CoinActor, Trans);
	}

	UPROPERTY(Category = "monster1", EditAnywhere, BlueprintReadWrite)
		float hp; // UI에게 넘겨주기 위해서 존재
	UFUNCTION()
		float Gethp()
	{
		return hp;
	}
	UFUNCTION()
		void Sethp(float _hp)
	{
		hp = _hp;
	}
	bool isoverlapboss; // 플레이어와 보스몬스터가 오버랩 됐음을 판단하는 bool 변수 

	AFire* FireActor;

	void Setbdeathfalse();

	template<typename EnumType>
	void SetAllSound(const TMap<EnumType, class USoundBase*>& _MapSound)
	{
		for (TPair<EnumType, USoundBase*> Pair : _MapSound)
		{
			AllSound.Add(static_cast<int>(Pair.Key), Pair.Value);
		}
	}

	template<typename EnumType>
	class USoundBase* GetSound(EnumType _Index)
	{
		if (false == AllSound.Contains(static_cast<int>(_Index)))
		{
			return nullptr;
		}

		return AllSound[static_cast<int>(_Index)];
	}

private:

	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		FName DataName = "Boss";

	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EAniState AIAniState = EAniState::Idle;
	UPROPERTY(Category = "BossMonster", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool isDeath = false;

	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(Category = "Coin", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGlobalCoin> CoinActor;
	
	UFUNCTION()
	void AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	
	FTimerHandle EffectDestroyTimerHandle; // 타이머 핸들변수

	void DestroyAttackEffect();

	AActor* AttackEffect = nullptr;

	UPROPERTY()
		class UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(Category = "Components", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<EAniState, class USoundBase*> MapSound;

	UPROPERTY(Category = "GlobalChracterValue", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TMap<int, class USoundBase*> AllSound;
};

