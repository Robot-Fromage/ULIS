#pragma once

#include <ULIS>
#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"

class FVectorGroup : public FVectorObject
{
    private:
        void DrawShape( FRectD& iRoi, uint64 iFlags ) { };
        FVectorObject* PickShape( double iX, double iY, double iRadius );
        FVectorObject* CopyShape();
        void UpdateShape();

    protected:

    public:
        ~FVectorGroup();
        FVectorGroup();
        FVectorGroup( std::string iName );
};
