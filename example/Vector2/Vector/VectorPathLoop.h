#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorPath.h"

class FVectorPathLoop : public FVectorPath
{
    protected :

    public:
        ~FVectorPathLoop();
        FVectorPathLoop();
        FVectorPathLoop( std::list<FVectorPoint*> iLoopPointist );
        void ShapeFromPointList( std::list<FVectorPoint*> iLoopPointist );
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        bool PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) { return false; };
        void Unselect( FVectorPoint* iPoint ) { };
        void PickPoint( double iX, double iY, double iRadius ) { };
};
