#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorCircle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );

    protected :
        double mRadius; 

    public:
        ~FVectorCircle();
        FVectorCircle();
        FVectorCircle( double iRadius );
        void SetRadius( double iRadius );
        double GetRadius( );
};
