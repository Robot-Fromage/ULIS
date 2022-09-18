#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

FVectorPoint::~FVectorPoint()
{
}

FVectorPoint::FVectorPoint()
{
}

FVectorPoint::FVectorPoint( double iX, double iY )
{
    Set( iX, iY );
}

double 
FVectorPoint::GetX()
{
    return mX;
}

double 
FVectorPoint::GetY()
{
    return mY;
}

void 
FVectorPoint::SetX( double iX )
{
    mX  = iX;
}

void 
FVectorPoint::SetY( double iY )
{
    mY = iY;
}

void 
FVectorPoint::Set( double iX, double iY )
{
    mX = iX;
    mY = iY;
}

void
FVectorPoint::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );
}
