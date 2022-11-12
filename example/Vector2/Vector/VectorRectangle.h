#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"

class FVectorRectangle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );

    protected :
        double mWidth;
        double mHeight; 

    public:
        ~FVectorRectangle();
        FVectorRectangle();
        FVectorRectangle( double iWidth, double iHeight );
        void SetSize( double iWidth, double iHeight );
        double GetWidth();
        double GetHeight();
};
