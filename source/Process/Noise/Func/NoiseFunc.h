// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BrownianNoiseMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation of a Noise type.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Vector.h"
#include <cassert>
#include <vector>
#include <cmath>
#include <cstdio>
#include <random>
#include <functional>
#include <iostream>
#include <fstream>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Value Noise Utilities
template<typename T = float>
inline T lerp(const T &lo, const T &hi, const T &t)
{
    return lo * ( 1 - t ) + hi * t;
}

inline
float smoothstep(const float &t)
{
    return t * t * ( 3 - 2 * t );
}

class FValueNoise
{
public:
    FValueNoise( unsigned seed = 2016 )
        : r{0}
        , permutationTable{0}
    {
        std::mt19937 gen(seed);
        std::uniform_real_distribution<float> distrFloat;
        auto randFloat = std::bind(distrFloat, gen);

        // create an array of random values and initialize permutation table
        for (unsigned k = 0; k < kMaxTableSize; ++k) {
            r[k] = randFloat();
            permutationTable[k] = k;
        }

        // shuffle values of the permutation table
        std::uniform_int_distribution<unsigned> distrUInt;
        auto randUInt = std::bind(distrUInt, gen);
        for (unsigned k = 0; k < kMaxTableSize; ++k) {
            unsigned i = randUInt() & kMaxTableSizeMask;
            std::swap(permutationTable[k], permutationTable[i]);
            permutationTable[k + kMaxTableSize] = permutationTable[k];
        }
    }

    float eval( FVec2F &p ) const
    {
        int xi = static_cast< int >( std::floor(p.x) );
        int yi = static_cast< int >( std::floor(p.y) );

        float tx = p.x - xi;
        float ty = p.y - yi;

        int rx0 = xi & kMaxTableSizeMask;
        int rx1 = (rx0 + 1) & kMaxTableSizeMask;
        int ry0 = yi & kMaxTableSizeMask;
        int ry1 = (ry0 + 1) & kMaxTableSizeMask;

        // random values at the corners of the cell using permutation table
        const float & c00 = r[permutationTable[permutationTable[rx0] + ry0]];
        const float & c10 = r[permutationTable[permutationTable[rx1] + ry0]];
        const float & c01 = r[permutationTable[permutationTable[rx0] + ry1]];
        const float & c11 = r[permutationTable[permutationTable[rx1] + ry1]];

        // remapping of tx and ty using the Smoothstep function
        float sx = smoothstep(tx);
        float sy = smoothstep(ty);

        // linearly interpolate values along the x axis
        float nx0 = lerp(c00, c10, sx);
        float nx1 = lerp(c01, c11, sx);

        // linearly interpolate the nx0/nx1 along they y axis
        return lerp(nx0, nx1, sy);
    }
    static const unsigned kMaxTableSize = 256;
    static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
    float r[kMaxTableSize];
    unsigned permutationTable[kMaxTableSize * 2];
};

ULIS_NAMESPACE_END

