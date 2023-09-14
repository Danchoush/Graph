
#include "MasterMind.h"
#include "Nodes.h"


AMasterMind::AMasterMind()
{
	PrimaryActorTick.bCanEverTick = true;
	_Count_X = 5;
	_Count_Y = 5;
	_Runners_Count = 3;
	_Runners_Speed = 20;
	_Radius = 2;
}


void AMasterMind::BeginPlay()
{
	Super::BeginPlay();
	ANodes* node = GetWorld()->SpawnActor<ANodes>();
	node->Init(_Count_X, _Count_Y, _Radius, _Runners_Count, _Runners_Speed, AMaterial);
	
}

