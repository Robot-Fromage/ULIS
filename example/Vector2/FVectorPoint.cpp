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
