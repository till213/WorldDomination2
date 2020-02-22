// MIT License - Copyright (c) 2020 Oliver Knoll

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

class UProceduralMeshComponent;

UCLASS()
class WORLDDOMINATION2_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrain();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent *TerrainMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
