#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"
#include "Vector/VectorPathCubic.h"

class FVectorPathBuilder : public FVectorObject
{
    private:
        double mCumulAngle;
        double mCumulAngleLimit;
        double mLastCubicAngleLimit;
        std::list<FVectorPoint*> mSamplePointList;
        std::list<FVectorLink*> mSampleLinkList;
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorLink*> mLinkList;
        FVectorSegmentCubic* Sample( FVectorPoint* iPoint, double iRadius, bool iEnforce );

        FVectorObject* CopyShape();
        void DrawShape( FRectD &iRoi, uint64 iFlags );
        FVectorObject* PickShape( double iX, double iY, double iRadius ) { return nullptr; };
        void UpdateShape() {};
        void FitSegment( FVectorSegmentCubic& iSegment, std::list<FVectorLink*>& iLinkList );

    protected :
        /*std::list<FVectorPoint*> mSmoothedPointList;*/
        FVectorPathCubic* mCubicPath;
        FVectorSegmentCubic* AppendPoint( double iX, double iY, double iRadius, bool iEnforce );
        void Round( FVectorSegmentCubic& iCubicSegment );
        void Sharp( FVectorSegmentCubic& iCubicSegment, FVec2D iEntryVector, FVec2D iExitVector );

    public:
       ~FVectorPathBuilder();
        //FVectorPathBuilder();
        FVectorPathBuilder( FVectorPathCubic* iCubicPath );
        FVectorSegment* AppendPoint( double iX, double iY, double iRadius );

        bool PickPoint( double iX, double iY, double iRadius, uint64 iSelectionFlags );
        void Unselect(FVectorPoint* iPoint);
        FVectorSegment* End( double iX, double iY, double iRadius, bool iClose );
        FVectorPathCubic* GetCubicPath( );
        FVectorLink* GetLastSampleLink();
        FVectorPoint* GetLastSamplePoint();

};
