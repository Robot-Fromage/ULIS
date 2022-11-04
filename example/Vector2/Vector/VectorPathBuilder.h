#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"
#include "Vector/VectorPathCubic.h"

class FVectorPathBuilder : public FVectorPath
{
    private:
        FVec2D mLastSmoothedSegmentVector;
        double mLastSmoothedSegmentVectorDistance;
        double mLastSmoothedAngle;

    protected :
        /*std::list<FVectorPoint*> mSmoothedPointList;*/
        FVectorPathCubic* mCubicPath;
        FVectorSegment* AppendPoint( double iX, double iY, double iRadius, bool iEnforce );

    public:
       ~FVectorPathBuilder();
        //FVectorPathBuilder();
        FVectorPathBuilder( FVectorPathCubic* iCubicPath );
        FVectorSegment* AppendPoint( double iX, double iY, double iRadius );
        void DrawShape(FBlock& iBlock,BLContext& iBLContext, FRectD &iRoi );
        bool PickShape(BLContext& iBLContext, double iX, double iY, double iRadius ) { return false; };
        void PickPoint( double iX, double iY, double iRadius );
        void Unselect(FVectorPoint* iPoint);
        FVectorSegment* End( double iX, double iY, double iRadius );
        FVectorPathCubic* GetCubicPath( );
};
