#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorHandle : public FVectorPoint
{
    private:

    protected:

    public:
        ~FVectorHandle();
        FVectorHandle();
        FVectorHandle( double iX, double iY );
};
