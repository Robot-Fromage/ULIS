#pragma once

#include <ULIS>
#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorRoot.h"

class FVectorEngine
{
    private:
        FVectorRoot mScene;
        FRectD mRoi;

    public:
        ~FVectorEngine();
        FVectorEngine();
        void Draw( FBlock& iBlock,BLContext& iBLContext );
        FVectorRoot& GetScene();
        void InvalidateRegion( double x, double y, double w, double h );
        void InvalidateRegion( FRectD& iRegion );
        FRectD& GetInvalidateRegion();
};
