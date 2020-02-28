// MIT License - Copyright (c) 2020 Oliver Knoll

#pragma once
#include <vector>
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
	std::vector<std::vector<float>> ProfileLinesX;
	std::vector<std::vector<float>> ProfileLinesZ;
	float MinElevation, MaxElevation;
	std::vector<std::vector<float>> HeightMap;
	std::vector<std::vector<FLinearColor>> TileColors;
	
	void RandomiseProfile();
	float Evaluate(float x) const;
	void CreateProfiles();
	void Elevate();
	void Normalise();
	void Colorise();

	static float CubicInterpolate(float y0,float y1, float y2,float y3, float mu);
};
