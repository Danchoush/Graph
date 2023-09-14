// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/LineBatchComponent.h"
#include "Nodes.h"
#include "Arrow.h"
#include "Runner.generated.h"



UCLASS()
class LAB_2_2_API ARunner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARunner();

	void CreateRunner(int Runners_Count, int Runners_Speed, TArray<FVector> Nodes, int Radius, int Links_Count, TArray<TArray<bool>> Links, int index,  FVector coord);
	virtual void Tick(float DeltaTime) override;
	void GetNewPath();
	void Dijkstra(int StartNode, int EndNode);
	UPROPERTY()
		UInstancedStaticMeshComponent* RunnerMesh;
	int Finish;
	int NowNodes;
	AArrow* arr;
	UMaterialInstanceDynamic*  Materia;

private:
	TArray<FVector> Graph;
	FVector Scale;
	int Speed;
	FVector NextNode;
	FVector PrevNode;

	int CurNode;
	TArray<FVector> Path;
	int Line_Count;
	TArray<TArray<bool>> Lines;
	int Index;
};
