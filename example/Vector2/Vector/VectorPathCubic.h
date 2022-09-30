#ifndef _FVECTORPATHCUBIC_H_
#define _FVECTORPATHCUBIC_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"

class FVectorPathCubic: public FVectorPath
{
    public:
        FVectorPathCubic();
        FVectorSegmentCubic* AppendPoint( FVectorPoint* iPoint );
        void Pick(double iX,double iY,double iRadius);
        void Unselect(FVectorPoint* iPoint);
};

#endif // _FVECTORPATHCUBIC_H_