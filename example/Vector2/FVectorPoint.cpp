#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPoint::~FVectorPoint()
{
}

FVectorPoint::FVectorPoint()
{
    Set( 0.0f, 0.0f );
}

FVectorPoint::FVectorPoint( double iX, double iY )
{
    Set( iX, iY );
}

FVec2D&
FVectorPoint::GetCoords()
{
    return mCoords;
}

double 
FVectorPoint::GetX()
{
    return mCoords.x;
}

double 
FVectorPoint::GetY()
{
    return mCoords.y;
}

void 
FVectorPoint::SetX( double iX )
{
    mCoords.x  = iX;
}

void 
FVectorPoint::SetY( double iY )
{
    mCoords.y = iY;
}

void 
FVectorPoint::Set( double iX, double iY )
{
    mCoords.x = iX;
    mCoords.y = iY;
}

void
FVectorPoint::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );
}

void
FVectorPoint::RemoveSegment( FVectorSegment* iSegment )
{
    mSegmentList.remove( iSegment );
}

uint32
FVectorPoint::GetSegmentCount( )
{
    return mSegmentList.size();
}

std::list<FVectorSegment*>&
FVectorPoint::GetSegmentList()
{
    return mSegmentList;
}

uint32
FVectorPoint::GetType()
{
    return POINT_TYPE_REGULAR; // default type;
}

double
FVectorPoint::GetRadius()
{
    return mRadius;
}

void
FVectorPoint::SetRadius( double iRadius
                       , bool iBuildSegments )
{
    mRadius = iRadius;
}

FVectorSegment*
FVectorPoint::GetLastSegment()
{
    return mSegmentList.back();
}

FVectorSegment*
FVectorPoint::GetFirstSegment()
{
    return mSegmentList.front();
}

static uintptr_t
GenerateSegmentID( FVectorSegment* iSegment, FVectorPoint* iP0, FVectorPoint* iP1 )
{
    uintptr_t xorPoint = reinterpret_cast<uintptr_t>(iP0) ^ reinterpret_cast<uintptr_t>(iP1);

    return xorPoint ^ reinterpret_cast<uintptr_t>(iSegment);
}

static bool seekPoint( std::list<FVectorPoint*>& iPointList
                     , FVectorPoint* iPoint )
{
    for( std::list<FVectorPoint*>::iterator it = iPointList.begin(); it != iPointList.end(); ++it )
    {
        if ( static_cast<FVectorPoint*>(*it) == iPoint )
        {
            return true;
        }
    }

    return false;
}

static bool seekSegment( std::list<uintptr_t>& iSegmentIDList
                       , uintptr_t iSegmentID )
{
    for( std::list<uintptr_t>::iterator it = iSegmentIDList.begin(); it != iSegmentIDList.end(); ++it )
    {
        if ( static_cast<uintptr_t>(*it) == iSegmentID )
        {
            return true;
        }
    }

    return false;
}

void
FVectorPoint::March( FVectorPointIntersection&  iInitiatorPoint
                   , FVectorSegment&       iCurrentSegment )
{
    static std::list<FVectorPoint*> loopPointList;
    static std::list<uintptr_t> iSegmentIDList;

    loopPointList.push_back ( this );

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);
        double t =  static_cast<FVectorPointIntersection*>(this)->GetT(*segment);
        FVectorPoint* forwardPoint = segment->GetNextPoint(t);
        FVectorPoint* backwardPoint = segment->GetPreviousPoint(t);

        if ( forwardPoint )
        {
            if ( seekPoint( loopPointList, forwardPoint ) == false )
            {
                iSegmentIDList.push_back( GenerateSegmentID( segment, this, forwardPoint) );
                forwardPoint->March( iInitiatorPoint, *segment );
                iSegmentIDList.pop_back();
            }
            else
            {
                if ( seekSegment( iSegmentIDList, GenerateSegmentID( segment, this, forwardPoint ) ) == false )
                {
                    uint64 loopID = FVectorPathLoop::GenerateID( loopPointList, forwardPoint );

                    if ( segment->GetPath()->GetPathLoopByID( loopID ) == nullptr )
                    {
                        FVectorPathLoop *loopPath = new FVectorPathLoop ( loopID, loopPointList, forwardPoint );

                        segment->GetPath()->AddLoop( loopPath );
                    } else printf("loop exists!!\n");
                }
            }
        }

        if( backwardPoint )
        {
            if( seekPoint( loopPointList, backwardPoint ) == false )
            {
                iSegmentIDList.push_back( GenerateSegmentID( segment, this, backwardPoint ) );
                backwardPoint->March( iInitiatorPoint, *segment );
                iSegmentIDList.pop_back();
            }
            else
            {
                if ( seekSegment( iSegmentIDList, GenerateSegmentID( segment, this, backwardPoint ) ) == false )
                {
                    uint64 loopID = FVectorPathLoop::GenerateID( loopPointList, backwardPoint );

                    if ( segment->GetPath()->GetPathLoopByID( loopID ) == nullptr )
                    {
                        FVectorPathLoop *loopPath = new FVectorPathLoop ( loopID, loopPointList, backwardPoint );

                        segment->GetPath()->AddLoop( loopPath );
                    } else printf("loop exists!!\n");
                }
            }
        }
    }

    loopPointList.pop_back ( );
}

FVectorSegment*
FVectorPoint::GetSegment( FVectorPoint& iOtherPoint )
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( ( ( &segment->GetPoint(0) == this ) && ( &segment->GetPoint(1) == &iOtherPoint ) ) 
          || ( ( &segment->GetPoint(1) == this ) && ( &segment->GetPoint(0) == &iOtherPoint ) ) )
        {
            return segment;
        }
    }

    return nullptr;
}
