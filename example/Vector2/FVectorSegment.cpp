#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegment::~FVectorSegment()
{
}

FVectorSegment::FVectorSegment( FVectorPoint* iPoint0
                              , FVectorPoint* iPoint1 )
{
    mPoint[0] = iPoint0;
    mPoint[1] = iPoint1;
}

FVectorPoint&
FVectorSegment::GetPoint( int iPointNum )
{
    return *mPoint[iPointNum];
}

FVectorPath*
FVectorSegment::GetPath()
{
    return mPath;
}

void
FVectorSegment::SetPath( FVectorPath* iPath )
{
    mPath = iPath;
}

double
FVectorSegment::GetStraightDistance()
{
    FVec2D vec = { mPoint[1]->GetX() - mPoint[0]->GetX(),
                   mPoint[1]->GetY() - mPoint[0]->GetY() };

    return vec.Distance();
}

void
FVectorSegment::DrawStructure( FBlock& iBlock
                             , BLContext& iBLContext
                             , FRectD &iRoi )
{

}

std::list<FVectorPointIntersection*>&
FVectorSegment::GetIntersectionPointList()
{
    return mIntersectionPointList;
}

bool
FVectorSegment::HasIntersectionPoint( FVectorPointIntersection& mIntersectionPoint )
{
    for( std::list<FVectorPointIntersection*>::iterator it = mIntersectionPointList.begin(); it != mIntersectionPointList.end(); ++it )
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*it);

        if ( intersectionPoint == &mIntersectionPoint ) 
        {
            return true;
        }
    }

    return false;
}

FVectorPoint*
FVectorSegment::GetNextPoint( double iT )
{
    // Search intersection point between this intersection point and the next segment point
    for(std::list<FVectorPointIntersection*>::iterator iter = mIntersectionPointList.begin(); iter != mIntersectionPointList.end(); ++iter)
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*iter);

        if( intersectionPoint->GetT(*this) > iT )
        {
            return intersectionPoint;
        }
    }

    return nullptr;
}

FVectorPoint*
FVectorSegment::GetPreviousPoint( double iT )
{
    // Search intersection point between this intersection point and the next segment point
    for( std::list<FVectorPointIntersection*>::iterator iter = mIntersectionPointList.begin(); iter != mIntersectionPointList.end(); ++iter )
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*iter);

        if( intersectionPoint->GetT(*this) < iT )
        {
            return intersectionPoint;
        }
    }

    return nullptr;
}

FVectorSegment*
FVectorSegment::GetNextSegment( )
{
    std::list<FVectorSegment*>& segmentList = mPoint[1]->GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != this ) 
        {
            return segment;
        }
    }

    return nullptr;
}

FVectorSegment* FVectorSegment::GetPreviousSegment( )
{
    std::list<FVectorSegment*>& segmentList = mPoint[0]->GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != this ) 
        {
            return segment;
        }
    }

    return nullptr;
}

void
FVectorSegment::Draw( FBlock& iBlock
                    , BLContext& iBLContext
                    , FRectD &iRoi )
{

}
