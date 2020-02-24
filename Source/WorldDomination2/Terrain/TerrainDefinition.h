 // MIT License - Copyright (c) 2020 Oliver Knoll

#pragma once

struct TerrainDefinition
{
    int nofTiles; // Must be a multiple of 'nofPatches'
    int nofPatches;

    float weight1;
    float weight2;
    float weight3;
    float weight4;

    // [-1.0, 1.0]
    float seaLevel;
};
