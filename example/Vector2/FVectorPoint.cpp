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

void
FVectorPoint::MarchForward( FVectorPointIntersection& iInitiatorPoint
                           , std::list<FVectorPoint*>& loopPointList
                           , std::list<FVectorPoint*>& intersectionPointList )
{
    FVectorPoint* previousLoopPoint = ( loopPointList.size() ) ? loopPointList.back() : nullptr;

    loopPointList.push_back ( this );

    if ( this->GetType() == POINT_TYPE_INTERSECTION )
    {
        intersectionPointList.push_back( this );
    }


    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPoint* nextPoint;
        double t = 0.0f;

        switch ( GetType() )
        {
            case POINT_TYPE_INTERSECTION :

                t =  static_cast<FVectorPointIntersection*>(this)->GetT(*segment);
            break;

            default : // POINT_TYPE_REGULAR
                t = ( this == &segment->GetPoint(0) ) ? 0.0f : 1.0f;
            break;
        }

        nextPoint = segment->GetNextPoint( t );

        if ( nextPoint )
        {
            // prevent going backwards (useful for interconnected paths going different in directions )
            if ( nextPoint != previousLoopPoint )
            {
                if ( nextPoint == &iInitiatorPoint )
                {
                    printf("forward intersected loop detected size:%d\n", loopPointList.size() );
                    FVectorPathLoop *loopPath = new FVectorPathLoop ( loopPointList );

                    iInitiatorPoint.AddLoop( loopPath );
                }
                else
                {
                    // detect loops within loop
                    if ( seekPoint ( intersectionPointList, nextPoint ) == false )
                    {
                        nextPoint->March( iInitiatorPoint );
                    }
                }
            }
        }
    }

    if( this->GetType() == POINT_TYPE_INTERSECTION )
    {
        intersectionPointList.pop_back ( );
    }

    loopPointList.pop_back ( );
}

void
FVectorPoint::MarchBackward( FVectorPointIntersection& iInitiatorPoint
                           , std::list<FVectorPoint*>& loopPointList
                           , std::list<FVectorPoint*>& intersectionPointList )
{
    FVectorPoint* previousLoopPoint = ( loopPointList.size() ) ? loopPointList.back() : nullptr;

    loopPointList.push_back ( this );

    if ( this->GetType() == POINT_TYPE_INTERSECTION )
    {
        intersectionPointList.push_back( this );
    }

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPoint* prevPoint;
        double t = 0.0f;

        switch ( GetType() )
        {
            case POINT_TYPE_INTERSECTION :
                t =  static_cast<FVectorPointIntersection*>(this)->GetT(*segment);
            break;

            default : // POINT_TYPE_REGULAR
                t = ( this == &segment->GetPoint(0) ) ? 0.0f : 1.0f;
            break;
        }

        prevPoint = segment->GetPreviousPoint( t );

        if ( prevPoint )
        {
            // prevent going backwards (useful for interconnected paths going different in directions )
            if ( prevPoint != previousLoopPoint )
            {
                if ( prevPoint == &iInitiatorPoint )
                {
                    printf("backward intersected loop detected size:%d\n", loopPointList.size() );
                    FVectorPathLoop *loopPath = new FVectorPathLoop ( loopPointList );

                    iInitiatorPoint.AddLoop( loopPath );
                }
                else
                {
                    // detect loops within loop
                    if ( seekPoint ( intersectionPointList, prevPoint ) == false )
                    {
                        prevPoint->March( iInitiatorPoint );
                    }
                }
            }
        }
    }

    if( this->GetType() == POINT_TYPE_INTERSECTION )
    {
        intersectionPointList.pop_back ( );
    }

    loopPointList.pop_back ( );
}

void
FVectorPoint::March( FVectorPointIntersection& iInitiatorPoint )
{
    static std::list<FVectorPoint*> loopPointList;
    // this is only for faster searching loops within loops
    static std::list<FVectorPoint*> intersectionPointList;

    MarchForward( iInitiatorPoint, loopPointList, intersectionPointList );
    /*MarchBackward( iInitiatorPoint, loopPointList, intersectionPointList );*/
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
}
