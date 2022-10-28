#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorHandle.h"

class FVectorHandleSegment : public FVectorHandle
{
    private:
        FVectorSegment* mParentSegment;

    protected:

    public:
        ~FVectorHandleSegment();
        FVectorHandleSegment();
        FVectorHandleSegment( double iX, double iY );
        uint32 GetType();
        void SetParent( FVectorSegment* iParentSegment );
        FVectorSegment* GetParent();
};
