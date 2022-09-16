#ifndef _FVECTORPATHCUBIC_H_
#define _FVECTORPATHCUBIC_H_

#include <ULIS>
#include <blend2d.h>
using namespace ::ULIS;

#include "FVectorSegmentCubic.h"
#include "FVectorPath.h"

class FVectorPathCubic: public FVectorPath
{
    public:
        FVectorPathCubic();
        FVectorSegment* AppendPoint( FVectorPoint* iPoint );
        void Pick(double iX,double iY,double iRadius);
        void Unselect(FVectorPoint* iPoint);
};

#endif // _FVECTORPATHCUBIC_H_