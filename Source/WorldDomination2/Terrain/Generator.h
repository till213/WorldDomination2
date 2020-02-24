// MIT License - Copyright (c) 2020 Oliver Knoll

#pragma once

#include "CoreMinimal.h"
#include "ProfileDefinition.h"
#include "TerrainDefinition.h"

/**
 * Terrain generator function.
 */
class WORLDDOMINATION2_API Generator
{
public:
	Generator();
	virtual ~Generator();

private:
	ProfileDefinition ProfileDefinition;
	TerrainDefinition TerrainDefinition;
	float *ProfileLinesX;
	float *ProfileLinesZ;
	float MinElevation, MaxElevation;
	float *VertexElevations;
	int *TileColors;
	
	void RandomiseProfile();
	float Evaluate(float x) const;
	void CreateProfiles();
	void Elevate();

	static float CubicInterpolate(float y0,float y1, float y2,float y3, float mu);
};
