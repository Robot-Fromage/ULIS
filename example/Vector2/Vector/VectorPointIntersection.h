#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorSegmentSub.h"

class FVectorPointIntersection : public FVectorPoint
{
    private:
        uint64 mIntersectionID;
        std::list<FVectorSegmentSub*> mSubSegmentList;
        double mT;

    protected:

    public:
        ~FVectorPointIntersection();
        FVectorPointIntersection();
        FVectorPointIntersection( double iX, double iY );
        uint32 GetType();
};
