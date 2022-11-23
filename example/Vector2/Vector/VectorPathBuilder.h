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
        double mCumulAngle;
        double mCumulAngleLimit;
        double mLastCubicAngleLimit;
        std::list<FVectorPoint*> mSamplePointList;
        std::list<FVectorSegment*> mSampleSegmentList;
        FVectorSegmentCubic* Sample(FVectorPoint* iPoint, double iRadius, bool iEnforce );

        FVectorObject* CopyShape();
        void DrawShape(FBlock& iBlock,BLContext& iBLContext,FRectD &iRoi);
        FVectorObject* PickShape(BLContext& iBLContext,double iX,double iY,double iRadius) { return nullptr; };
        void UpdateShape() {};

    protected :
        /*std::list<FVectorPoint*> mSmoothedPointList;*/
        FVectorPathCubic* mCubicPath;
        FVectorSegment* AppendPoint( double iX, double iY, double iRadius, bool iEnforce );
        void Round( FVectorSegmentCubic& cubicSegment, FVec2D iFirstVector, FVec2D iLastVector );

    public:
       ~FVectorPathBuilder();
        //FVectorPathBuilder();
        FVectorPathBuilder( FVectorPathCubic* iCubicPath );
        FVectorSegment* AppendPoint( double iX, double iY, double iRadius );

        bool PickPoint( double iX, double iY, double iRadius, uint64 iSelectionFlags );
        void Unselect(FVectorPoint* iPoint);
        FVectorSegment* End( double iX, double iY, double iRadius, bool iClose );
        FVectorPathCubic* GetCubicPath( );
        FVectorSegment* GetLastSampleSegment();
        FVectorPoint* GetLastSamplePoint();

};
