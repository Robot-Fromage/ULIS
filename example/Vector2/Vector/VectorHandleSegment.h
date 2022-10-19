#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorHandle.h"

class FVectorHandleSegment : public FVectorHandle
{
    private:
    protected:

    public:
        static  const uint32 HANDLE_TYPE_SEGMENT = 2;

        ~FVectorHandleSegment();
        FVectorHandleSegment();
        FVectorHandleSegment( double iX, double iY );
        uint32 GetType();
};
