#ifndef _FVECTORPATHCUBIC_H_
#define _FVECTORPATHCUBIC_H_

#include <ULIS>
#include <blend2d.h>

#include "Vector/FVectorSegmentCubic.h"
#include "Vector/FVectorPath.h"

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorPathCubic: public FVectorPath
{
    public:
        FVectorPathCubic();
        FVectorSegment* AppendPoint( FVectorPoint* iPoint );
        void Pick(double iX,double iY,double iRadius);
        void Unselect(FVectorPoint* iPoint);
};

ULIS_NAMESPACE_END

#endif // _FVECTORPATHCUBIC_H_