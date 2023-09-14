// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes.h"
#include "Links.h"
#include "Runner.h"
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))


// Sets default values
ANodes::ANodes()
{

	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NodeMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedGraphMesh"));
	NodeMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	NodeMesh->SetStaticMesh(sphereMesh.Object);
	NodeMesh->SetCastShadow(false);
}

int ANodes::GetNeighbour(int CurrentNode)
{
	TArray<int> Neighbours;
	int NeighbourNode = 0;

	for (int y = -1; y <= 1; y++)
		for (int x = -1; x <= 1; x++) {

			int cx = CurrentNode % Count_X, cy = CurrentNode / Count_X;
			if (cx + x < 0 || cy + y < 0 || cx + x >= Count_X || cy + y >= Count_Y)
				continue;

			if ((abs(x) + abs(y)) / 2 != 1)
				continue;

			NeighbourNode = (cx + x) + (cy + y) * Count_X;
			if (Links[CurrentNode][NeighbourNode] || Links[NeighbourNode][CurrentNode])
				continue;

			if (x != 0 && y != 0) {

				if (Links[cx + (cy + y) * Count_X][(cx + x) + cy * Count_X] || Links[(cx + x) + cy * Count_X][cx + (cy + y) * Count_X])
					continue;
			}

			Neighbours.Add(NeighbourNode);
		}

	if (Neighbours.Num() == 0) return -1;
	return Neighbours[FMath::RandRange(0, Neighbours.Num() - 1)];

}

int ANodes::GetUnstackedNeighbour(int CurrentNode, TArray<bool>& NodesMarks)
{
	TArray<int> Neighbours;
	int NeighbourNode;

	for (int y = -1; y <= 1; y++)
		for (int x = -1; x <= 1; x++) {

			int cx = CurrentNode % Count_X, cy = CurrentNode / Count_X;
			if (cx + x < 0 || cy + y < 0 || cx + x >= Count_X || cy + y >= Count_Y)
				continue;

			NeighbourNode = (cx + x) + (cy + y) * Count_X;
			if (Links[CurrentNode][NeighbourNode] || Links[NeighbourNode][CurrentNode])
				continue;

			if (x != 0 && y != 0) {

				if (Links[cx + (cy + y) * Count_X][(cx + x) + cy * Count_X] || Links[(cx + x) + cy * Count_X][cx + (cy + y) * Count_X])
					continue;
			}

			if (NodesMarks[NeighbourNode])
				continue;

			Neighbours.Add(NeighbourNode);
		}

	if (Neighbours.Num() == 0) return -1;
	return Neighbours[FMath::RandRange(0, Neighbours.Num() - 1)];
}


void ANodes::CreateGrid()
{
	UMaterialInstanceDynamic* ColorMaterialNodes = UMaterialInstanceDynamic::Create(colMat, NULL);
	FLinearColor colori = FLinearColor(192, 192, 192);
	ColorMaterialNodes->SetVectorParameterValue(TEXT("Color"), colori);
	NodeMesh->SetMaterial(0, ColorMaterialNodes);

	for (int y = 0; y < Count_Y; y++) {
		for (int x = 0; x < Count_X; x++) {
			FVector NodeV = FVector(x * Step * Radius * 4, y * Step * Radius * 4, 0);
			int offset = FMath::RandRange(-Step * Radius / 2, Step * Radius / 2);
			NodeV.X += offset;
			NodeV.Y += offset;
			NodeV.Z += offset * 2;
			Nodes.Add(NodeV);
		}
	}

	TArray<int> NodesStack;
	TArray<bool> NodesMarks;
	Links_Count = 0;
	Links.Init({}, Count_X * Count_Y);

	for (int x = 0; x < Nodes.Num(); ++x) {
		Links[x].Init(false, Count_X * Count_Y);
		NodeMesh->AddInstance(FTransform(FRotator(), Nodes[x], FVector(Radius, Radius, Radius)));
		NodesMarks.Add(false);
	}

	int CurrentNode = FMath::RandRange(0, Nodes.Num() - 1);
	int NeighbourNode;

	NodesStack.Push(CurrentNode);
	NodesMarks[CurrentNode] = true;

	while (NodesStack.Num() > 0) {
		CurrentNode = NodesStack.Pop();

		NeighbourNode = this->GetUnstackedNeighbour(CurrentNode, NodesMarks);
		if (NeighbourNode == -1) continue;

		NodesMarks[NeighbourNode] = true;

		Links[CurrentNode][NeighbourNode] = true;
		Links[NeighbourNode][CurrentNode] = true;
		NodesStack.Push(CurrentNode);
		NodesStack.Push(NeighbourNode);
		++Links_Count;
	}

	int maxLinks = 3 * Count_X * Count_Y - 3;
	int addedLinks = maxLinks - Links_Count;

	for (int i = 0; i <= addedLinks; ++i)
	{
		CurrentNode = FMath::RandRange(0, Nodes.Num() - 1);
		NeighbourNode = this->GetNeighbour(CurrentNode);

		if (NeighbourNode == -1)
		{
			continue;
		}
		++Links_Count;
		Links[CurrentNode][NeighbourNode] = true;
		Links[NeighbourNode][CurrentNode] = true;
	}
}



void ANodes::CreateLinks() {
	ALinks* link = GetWorld()->SpawnActor<ALinks>();
	FLinearColor color = FColor::Blue;
	for (int x = 0; x < Count_X * Count_Y; x++) {
		for (int y = 0; y < x; y++) {
			if (Links[x][y])
				link->CreateLink(Nodes[x], Nodes[y], color, Radius);
		}
	}
}


void ANodes::CreateRunners() {
	TArray<ARunner*> run;

	for (int i = 0; i < Runners_Count; i++) {
		int NowNode = FMath::RandRange(0, Nodes.Num() - 1);
		int Finish = FMath::RandRange(0, Nodes.Num() - 1);
		while (NowNode == Finish)
			Finish = FMath::RandRange(0, Nodes.Num() - 1);
		run.Add(GetWorld()->SpawnActor<ARunner>(Nodes[NowNode], FRotator(0, 0, 0)));
		UMaterialInstanceDynamic* ColorMaterial = UMaterialInstanceDynamic::Create(colMat, NULL);
		FLinearColor colori = FLinearColor::MakeFromHSV8(i*255/Runners_Count,192,192);
		ColorMaterial->SetVectorParameterValue(TEXT("Color"), colori); 
		run[i]->Finish = Finish;
		run[i]->NowNodes = NowNode;
		run[i]->Materia = ColorMaterial;
		run[i]->CreateRunner(Runners_Count, Runners_Speed, Nodes, Radius, Links_Count, Links, 1, FVector(0, 0, 0));
		

	}
}

void ANodes::Init(int _Count_X, int _Count_Y, float _Radius, int _Runners_Count, float _Runners_Speed, UMaterial* AMaterial)
{
	colMat = AMaterial;
	Step = 100;
	Count_X = _Count_X;
	Count_Y = _Count_Y;
	Runners_Count = _Runners_Count;
	Runners_Speed = _Runners_Speed;
	Radius = _Radius;
	CreateGrid();
	CreateLinks();
	CreateRunners();


	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Orange, FString::Printf(TEXT("Number of Links: %d"), Links_Count));
	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Orange, FString::Printf(TEXT("Number of Nodes: %d"), Nodes.Num()));
	
	
}
