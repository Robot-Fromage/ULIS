#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorSegmentSub : public FVectorSegment
{
    protected:

    public:
        ~FVectorSegmentSub();
        FVectorSegmentSub();
        FVectorSegmentSub( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
};
