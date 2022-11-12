#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorPath;
class FVectorPathLoop;
class FVectorSegment;
class FVectorSegmentCubic;

class FVectorPointIntersection : public FVectorPoint
{
    struct FIntersection {
        FVec2D position;
        double t;
    };

    protected:
        uint64 mIntersectionID;
        // map for intersection positions
        std::map<FVectorSegment*, FIntersection> mTMap;

    public:
        ~FVectorPointIntersection();
        FVectorPointIntersection();
        uint32 GetType();
        FVec2D GetPosition( FVectorSegment& );
        double GetT( FVectorSegment& );
        void Draw( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        void AddSegment( FVectorSegmentCubic* iSegment, double t );
        // overloaded
        FVectorSegment* GetSegment( FVectorPoint& iOtherPoint );
};
