#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorSegment.h"

class FVectorPoint
{
    private:

    protected:
        FVec2D mCoords;
        std::list<FVectorSegment*> mSegmentList;

    public:
        ~FVectorPoint();
        FVectorPoint();
        FVectorPoint( double iX, double iY );
        void AddSegment( FVectorSegment* iSegment );
        void RemoveSegment( FVectorSegment* iSegment );
        FVec2D& GetCoords();
        double GetX();
        double GetY();
        void SetX( double iX );
        void SetY( double iY );
        void Set( double iX, double iY );
        uint32 GetSegmentCount();
        std::list<FVectorSegment*>& GetSegmentList();
        virtual uint32 GetType();
};
