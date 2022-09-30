#ifndef _FVECTORPATHBUILDER_H_
#define _FVECTORPATHBUILDER_H_

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

    protected :
        std::list<FVectorPoint*> mSmoothedPointList;
        FVectorPathCubic* mCubicPath;
        FVectorSegment* AppendPoint( double iX, double iY, bool iEnforce );

    public:
       ~FVectorPathBuilder();
        FVectorPathBuilder();
        FVectorSegment* AppendPoint( double iX, double iY );
        void DrawShape(FBlock& iBlock,BLContext& iBLContext);
        bool PickShape(BLContext& iBLContext,double iX,double iY) { return false; };
        void Pick(double iX,double iY,double iRadius);
        void Unselect(FVectorPoint* iPoint);
        FVectorSegment* Close( double iX, double iY );
        FVectorPathCubic* GetSmoothedPath( );
};

#endif // _FVECTORPATHBUILDER_H_