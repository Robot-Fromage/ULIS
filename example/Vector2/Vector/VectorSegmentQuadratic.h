#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"
#include "Vector/VectorSegment.h"

class FVectorSegmentQuadratic : public FVectorSegment
{
    private:
        FVectorPoint mCtrlPoint;

    public:
        ~FVectorSegmentQuadratic();
        FVectorSegmentQuadratic();
        FVectorSegmentQuadratic( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorSegmentQuadratic( FVectorPoint* iPoint0, double iCtrlx, double iCtrly, FVectorPoint* iPoint1 );
        void Draw( FBlock& iBlock, BLContext& iBLContext );
};
