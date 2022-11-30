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
        BLImage* mBLImage;
        BLImage* mBLMask;

    public:
        static BLContext& GetBLContext();
        BLImage& GetBLImage();
        ~FVectorEngine();
        FVectorEngine( double iWidth, double iHeight );
        void Render( FBlock& iBlock );
        FVectorRoot& GetScene();
        /*void Init( double iWidth, double iHeight );*/
        void InvalidateRegion( double x, double y, double w, double h );
        void InvalidateRegion( FRectD& iRegion );
        FRectD& GetInvalidateRegion();
};
