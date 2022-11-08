#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorPoint;
class FVectorPointIntersection;

class FVectorSegment
{
    protected:
        FVectorPoint* mPoint[2];
        std::list<FVectorPointIntersection*> mIntersectionPointList;

    public:
        ~FVectorSegment();
        FVectorSegment();
        FVectorSegment( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorPoint& GetPoint( int iPointNum );
        virtual void Draw( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        virtual void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        double GetStraightDistance();
        FVectorSegment* GetPreviousSegment();
        FVectorSegment* GetNextSegment();
        std::list<FVectorPointIntersection*>& GetIntersectionPointList();
        bool HasIntersectionPoint( FVectorPointIntersection& mIntersectionPoint );
        FVectorPoint* GetNextPoint( double iT );
        FVectorPoint* GetPreviousPoint( double iT );
};
