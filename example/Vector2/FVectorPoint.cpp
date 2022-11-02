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

FVectorPoint*
FVectorPoint::GetNextPoint( FVectorSegment& iCurrentSegment, double iT )
{
    FVectorPoint& endPoint = iCurrentSegment.GetPoint( 1 );

    // Search intersection point between this intersection point and the next segment point
    std::list<FVectorPointIntersection*>& intersectionPointList = iCurrentSegment.GetIntersectionPointList();

    for( std::list<FVectorPointIntersection*>::iterator iter = intersectionPointList.begin(); iter != intersectionPointList.end(); ++iter )
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*iter);

        if( intersectionPoint->GetT() > iT )
        {
            return intersectionPoint;
        }
    }

    return ( this == &endPoint ) ? nullptr : &endPoint;
}

void
FVectorPoint::March( FVectorPointIntersection& iInitiatorPoint )
{
    static std::list<FVectorPoint*> loopPointist;

    loopPointist.push_back ( this );

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPoint* nextPoint;
        double t = 0.0f;

        switch ( GetType() )
        {
            case POINT_TYPE_INTERSECTION :
                t =  static_cast<FVectorPointIntersection*>(this)->GetT();
            break;

            default : // POINT_TYPE_REGULAR
                t = ( this == &segment->GetPoint(0) ) ? 0.0f : 1.0f;
            break;
        }

        nextPoint = GetNextPoint( *segment, t );

        if ( nextPoint )
        {
            if ( nextPoint == &iInitiatorPoint )
            {
                printf("intersected loop detected size:%d\n", loopPointist.size() );
                FVectorPathLoop *loopPath = new FVectorPathLoop ( loopPointist );

                iInitiatorPoint.AddLoop( loopPath );
            }
            else
            {
                nextPoint->March( iInitiatorPoint );
            }
        }
    }

    loopPointist.pop_back ( );
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
