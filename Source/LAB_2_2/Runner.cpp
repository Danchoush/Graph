// Fill out your copyright notice in the Description page of Project Settings.


#include "Runner.h"
#include "Nodes.h"
#include "Arrow.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))


ARunner::ARunner()
{

	PrimaryActorTick.bCanEverTick = true;

	RunnerMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedRunnerMesh"));
	RunnerMesh->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	RunnerMesh->SetStaticMesh(cubeMesh.Object);
	RunnerMesh->SetCastShadow(false);

	Finish = 0;
	CurNode = 0;
	NextNode = FVector(0, 0, 0);
	PrevNode = FVector(0, 0, 0);
}

void ARunner::CreateRunner(int Runners_Count, int Runners_Speed, TArray<FVector> Nodes, int Radius, int Links_Count, TArray<TArray<bool>> Links, int index,  FVector coord)
{
	RunnerMesh->SetMaterial(0, Materia);
	Lines = Links;
	Line_Count = Links_Count;
	Graph = Nodes;
	Scale = FVector(Radius, Radius, Radius);
	Speed = Runners_Speed;
	
	arr = GetWorld()->SpawnActor<AArrow>(FVector(0,0,0), FRotator(0, 0, 0));
	arr->Radius = Scale.X;
	arr->Materia = Materia;

	Dijkstra(NowNodes, Finish);
	RunnerMesh->AddInstance(FTransform(FRotator(), FVector(0, 0, 0), Scale));
	arr->CreateArrow(Path);

}


void ARunner::GetNewPath()
{
	int StartNode = Finish;
	Finish = FMath::RandRange(0, Graph.Num() - 1);
	while (StartNode == Finish)
		Finish = FMath::RandRange(0, Graph.Num() - 1);
	Dijkstra(StartNode, Finish);

}

void ARunner::Dijkstra(int StartNode, int EndNode) {
	TArray<int> MinPath;
	TArray<int> Parents;
	TArray<int> NodesStack;
	TArray<bool> NodesMarks;
	int NowNode;

	MinPath.Init(Line_Count * 2, Line_Count);
	Parents.Init(-1, Line_Count);
	NodesMarks.Init(false, Line_Count);

	NodesStack.Add(StartNode);
	MinPath[StartNode] = 0;

	while (NodesStack.Num() > 0) {
		NowNode = NodesStack.Pop();
		NodesMarks[NowNode] = true;
		for (int x = 0; x < Graph.Num(); x++) {
			if (!Lines[x][NowNode]) continue;

			if (MinPath[x] > MinPath[NowNode] + 1) {
				MinPath[x] = MinPath[NowNode] + 1;
				Parents[x] = NowNode;
			}

			if (!NodesMarks[x]) {
				NodesStack.Add(x);
			}
		}
	}

	NowNode = EndNode;
	Path.Empty();
	while (NowNode != StartNode) {
		Path.Insert(Graph[NowNode], 0);
		NowNode = Parents[NowNode];
	}
	Path.Insert(Graph[NowNode], 0);
}




void ARunner::Tick(float DeltaTime)
{

	if (CurNode != Path.Num()) {
		FVector direction = Path[CurNode] - this->GetActorLocation();
		if (direction.Size() > Speed * DeltaTime) {
			this->SetActorLocation(this->GetActorLocation() + direction.GetSafeNormal() * Speed * DeltaTime);
		}
		else {
			this->SetActorLocation(Path[CurNode]);
			if (CurNode>0)
				arr->DestroyArrow();
			CurNode += 1;
		}
	}
	else {
		GetNewPath();
		CurNode = 0;
		arr->CreateArrow(Path);
	}

	
}

