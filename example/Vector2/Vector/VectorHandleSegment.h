#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorHandle.h"

class FVectorHandleSegment : public FVectorHandle
{
    private:
        FVectorSegment& mParentSegment;

    protected:

    public:
        ~FVectorHandleSegment();
        FVectorHandleSegment( FVectorSegment& iParentSegment, double iX, double iY );
        uint32 GetType();
        FVectorSegment& GetParent();
        void SetX( double iX );
        void SetY( double iY );
        void Set( double iX,double iY );
};
