#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"

class FVectorCircle : public FVectorPathCubic
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );
        void UpdateShape();
 
        FVectorPointCubic* mCubicPoint[4];
        FVectorSegmentCubic* mCubicSegment[4];

    protected :
        double mRadiusX; 
        double mRadiusY; 

    public:
        ~FVectorCircle();
        FVectorCircle( std::string iName );
        FVectorCircle( std::string iName, double iRadius );
        FVectorCircle( std::string iName, double iRadiusX, double iRadiusY );
        void SetRadius( double iRadius );
        void SetRadius( double iRadiusX, double iRadiusY );
        double GetRadiusX();
        double GetRadiusY();
        FVectorObject* CopyShape();
        FVectorPathCubic* Convert();
};
