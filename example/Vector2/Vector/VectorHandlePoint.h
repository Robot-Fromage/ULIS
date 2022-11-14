#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorHandle.h"

class FVectorHandlePoint : public FVectorHandle
{
    private:
        FVectorPoint& mParentPoint;

    protected:

    public:
        ~FVectorHandlePoint();
        FVectorHandlePoint( FVectorPoint& iParentPoint );
        uint32 GetType();
        FVectorPoint& GetParent();
};
