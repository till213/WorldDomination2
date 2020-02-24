// MIT License - Copyright (c) 2020 Oliver Knoll

#include "Generator.h"
#include "Math/UnrealMathUtility.h"
#include <limits>

Generator::Generator()
  : ProfileLinesX(nullptr),
    ProfileLinesZ(nullptr),
    VertexElevations(nullptr)
{
}

Generator::~Generator()
{
    if (ProfileLinesX != nullptr) {
        delete ProfileLinesX;
    }
    if (ProfileLinesZ != nullptr) {
        delete ProfileLinesZ;
    }
    if (VertexElevations != nullptr) {
        delete VertexElevations;
    }
}

// Private

void Generator::RandomiseProfile()
{
    ProfileDefinition.amplitude1 = FMath::FRand();
    ProfileDefinition.frequency1 = 1.0f;
    ProfileDefinition.offset1 = FMath::FRand() * PI * 2;

    ProfileDefinition.amplitude2 = FMath::FRand();
    ProfileDefinition.frequency2 = 2;
    ProfileDefinition.offset2 = FMath::FRand() * PI * 2;

    ProfileDefinition.amplitude3 = FMath::FRand();
    ProfileDefinition.frequency3 = 4;
    ProfileDefinition.offset3 = FMath::FRand() * PI * 2;

    ProfileDefinition.amplitude4 = FMath::FRand();
    ProfileDefinition.frequency4 = 8;
    ProfileDefinition.offset4 = FMath::FRand() * PI * 2;
}

float Generator::Evaluate(float x) const
{
    float y = ProfileDefinition.weight1 * (ProfileDefinition.amplitude1 * FMath::Sin (ProfileDefinition.frequency1 * (x + ProfileDefinition.offset1)))
            + ProfileDefinition.weight2 * (ProfileDefinition.amplitude2 * FMath::Sin (ProfileDefinition.frequency2 * (x + ProfileDefinition.offset2)))
            + ProfileDefinition.weight3 * (ProfileDefinition.amplitude3 * FMath::Sin (ProfileDefinition.frequency3 * (x + ProfileDefinition.offset3)))
            + ProfileDefinition.weight4 * (ProfileDefinition.amplitude4 * FMath::Sin (ProfileDefinition.frequency4 * (x + ProfileDefinition.offset4)));
    return y;
}

void Generator::CreateProfiles()
{
    int NofPoints = TerrainDefinition.nofTiles + 1;
    float x0 = -PI;
    float z0 = -PI;
    float dx = (2.0f * PI) / TerrainDefinition.nofTiles;
    float dz = dx;

    int nofProfileLines = TerrainDefinition.nofPatches + 1;

    ProfileDefinition.weight1 = TerrainDefinition.weight1;
    ProfileDefinition.weight2 = TerrainDefinition.weight2;
    ProfileDefinition.weight3 = TerrainDefinition.weight3;
    ProfileDefinition.weight4 = TerrainDefinition.weight4;

    // Profile lines X
    ProfileLinesX = new float[nofProfileLines * NofPoints];
    for (int p = 0; p < nofProfileLines - 1; ++p) {
        RandomiseProfile();
        float x = x0;
        for (int u = 0; u < NofPoints; ++u) {
            ProfileLinesX[p * NofPoints + u] = Evaluate(x);
            x += dx;
        } // x
    }

    // Profile lines Z
    ProfileLinesZ = new float[nofProfileLines * NofPoints];
    for (int o = 0; o < nofProfileLines - 1; ++o) {
        RandomiseProfile();
        float z = z0;
        for (int v = 0; v < NofPoints; ++v) {
            ProfileLinesZ [o * NofPoints + v] = Evaluate(z);
            z += dz;
        } // z
    }

    // Last profile line identical to first one
    for (int u = 0; u < NofPoints; ++u) {
        ProfileLinesX [(nofProfileLines - 1) * NofPoints + u] = ProfileLinesX [0, u];
    }

    // Last profile line identical to first one
    for (int v = 0; v < NofPoints; ++v) {
        ProfileLinesZ [(nofProfileLines - 1) * NofPoints + v] = ProfileLinesZ [0, v];
    }

}

void Generator::Elevate()
{
    int nofPoints = TerrainDefinition.nofTiles + 1;
    VertexElevations = new float[nofPoints * nofPoints];
    int nofTilesPerPatch = TerrainDefinition.nofTiles / TerrainDefinition.nofPatches;
    int nofContourLines = TerrainDefinition.nofPatches + 1;

    MaxElevation = std::numeric_limits<float>::min();
    MinElevation = std::numeric_limits<float>::max();
    // Interpolate profile lines across X- and Z-direction (Y is up)
    for (int p = 0; p < TerrainDefinition.nofPatches; ++p) {
        for (int v = p * nofTilesPerPatch; v < p * nofTilesPerPatch + nofTilesPerPatch; ++v) {
            for (int o = 0; o < TerrainDefinition.nofPatches; ++o) {					
                float t = (float)(v % nofTilesPerPatch) / (float)nofTilesPerPatch;
                for (int u = o * nofTilesPerPatch; u < o * nofTilesPerPatch + nofTilesPerPatch; ++u) {
                    float s = (float)(u % nofTilesPerPatch) / (float)nofTilesPerPatch;
            
                    int p1 = ((p - 1) % nofContourLines + nofContourLines) % nofContourLines;
                    float y1 = CubicInterpolate(ProfileLinesX[p1, u],
                                                ProfileLinesX[p, u],
                                                ProfileLinesX[p + 1, u],
                                                ProfileLinesX[(p + 2) % nofContourLines, u],
                                                t);
                    p1 = ((o - 1) % nofContourLines + nofContourLines) % nofContourLines;
                    float y2 = CubicInterpolate(ProfileLinesZ[p1, v],
                                                ProfileLinesZ[o, v],
                                                ProfileLinesZ[o + 1, v],
                                                ProfileLinesZ[(o + 2) % nofContourLines, v],
                                                s);
                    float y = y1 + y2;
                    VertexElevations[u * nofPoints + v] = y;
                    if (y > MaxElevation) {
                        MaxElevation = y;
                    } else if (y < MinElevation) {
                        MinElevation = y;
                    }
                }
            }
        }
    }

    // Border lines
    for (int u = 0; u < nofPoints; ++u) {
        VertexElevations [u, nofPoints - 1] = VertexElevations [u, 0];
    }
    for (int v = 0; v < nofPoints; ++v) {
        VertexElevations [nofPoints - 1, v] = VertexElevations [0, v];
    }
}

float Generator::CubicInterpolate(float y0,float y1, float y2,float y3, float mu)
{
    // http://paulbourke.net/miscellaneous/interpolation/
    float a0, a1, a2, a3, mu2;

    mu2 = mu * mu;

    // Catmull-Rom spline
    a0 = -0.5f * y0 + 1.5f * y1 - 1.5f * y2 + 0.5f * y3;
    a1 = y0 - 2.5f * y1 + 2.0f * y2 - 0.5f * y3;
    a2 = -0.5f * y0 + 0.5f * y2;
    a3 = y1;

    return (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}