#include <blend2d.h>
#include "Vector/Vector.h"

FVectorHandleSegment::~FVectorHandleSegment()
{
}

FVectorHandleSegment::FVectorHandleSegment( FVectorSegment& iParentSegment, double iX, double iY )
    : FVectorHandle( iX, iY )
   , mParentSegment ( iParentSegment )
{

}

uint32
FVectorHandleSegment::GetType()
{
    return FVectorPoint::POINT_TYPE_HANDLE_SEGMENT;
}

FVectorSegment&
FVectorHandleSegment::GetParent()
{
    return mParentSegment;
}

void 
FVectorHandleSegment::SetX( double iX )
{
    mCoords.x  = iX;

    mParentSegment.Invalidate();
}

void 
FVectorHandleSegment::SetY( double iY )
{
    mCoords.y = iY;

    mParentSegment.Invalidate();
}

void 
FVectorHandleSegment::Set( double iX, double iY )
{
    mCoords.x = iX;
    mCoords.y = iY;

    mParentSegment.Invalidate();
}
