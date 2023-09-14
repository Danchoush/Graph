// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterMind.generated.h"

UCLASS()
class LAB_2_2_API AMasterMind : public AActor
{
	GENERATED_BODY()
	
	AMasterMind();
	UPROPERTY(EditAnywhere)
		int _Count_X;
	UPROPERTY(EditAnywhere)
		int _Count_Y;
	UPROPERTY(EditAnywhere)
		float _Radius;
	UPROPERTY(EditAnywhere)
		int _Runners_Count;
	UPROPERTY(EditAnywhere)
		float _Runners_Speed;

	UPROPERTY(EditAnywhere)
		UMaterial* AMaterial ;

protected:
	
	virtual void BeginPlay() override;

};
