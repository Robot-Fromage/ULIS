#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorPath;
class FVectorPathLoop;

class FVectorPointIntersection : public FVectorPoint
{
    protected:
        uint64 mIntersectionID;
        std::list<FVectorPathLoop*> mLoopList; // list of loops
        double mT;

    public:
        ~FVectorPointIntersection();
        FVectorPointIntersection();
        FVectorPointIntersection( double iX, double iY, double iT );
        uint32 GetType();
        double GetT();
        void AddLoop( FVectorPathLoop* iLoop );
        void DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
};
