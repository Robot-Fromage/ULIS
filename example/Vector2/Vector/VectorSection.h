#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorSection
{
    protected:
        FVectorSegment& mSegment;
        FVectorPoint* mPoint[2];
        std::list<FVectorLoop*> mLoopList;

    public:
        ~FVectorSection();
        FVectorSection( FVectorSegment& iSegment, FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorSegment& GetSegment();
        FVectorPoint* GetPoint( int iNum );
        void AddLoop( FVectorLoop* iLoop );
        void RemoveLoop( FVectorLoop* iLoop );
        std::list<FVectorLoop*>& GetLoopList();
};
