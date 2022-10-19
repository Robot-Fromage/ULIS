#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorHandle.h"

class FVectorHandlePoint : public FVectorHandle
{
    private:
        FVectorPoint* mParentPoint;

    protected:

    public:
        static  const uint32 HANDLE_TYPE_POINT = 1;

        ~FVectorHandlePoint();
        FVectorHandlePoint();
        FVectorHandlePoint( FVectorPoint* iParentPoint );
        uint32 GetType();
        void SetParent( FVectorPoint* iParentPoint );
        FVectorPoint* GetParent();
};
