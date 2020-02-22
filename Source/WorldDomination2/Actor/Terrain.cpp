// MIT License - Copyright (c) 2020 Oliver Knoll

#include "Terrain.h"
#include <ProceduralMeshComponent.h>

// Sets default values
ATerrain::ATerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TerrainMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Terrain"));
	RootComponent = TerrainMesh;
	// New in UE 4.17, multi-threaded PhysX cooking.
	TerrainMesh->bUseAsyncCooking = true;
}

// Called when the game starts or when spawned
void ATerrain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

