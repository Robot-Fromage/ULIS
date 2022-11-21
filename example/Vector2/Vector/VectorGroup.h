#pragma once

#include <ULIS>
#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"

class FVectorGroup : public FVectorObject
{
    private:

    protected:

    public:
        ~FVectorGroup();
        FVectorGroup();
        FVectorGroup( std::string iName );

        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi ) { };
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );
        FVectorObject* CopyShape();
        void UpdateShape();
};
