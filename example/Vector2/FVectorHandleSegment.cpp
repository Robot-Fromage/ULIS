#include <blend2d.h>
#include "Vector/Vector.h"

FVectorHandleSegment::~FVectorHandleSegment()
{
}

FVectorHandleSegment::FVectorHandleSegment()
    : FVectorHandle()
{
}

FVectorHandleSegment::FVectorHandleSegment( double iX, double iY )
    : FVectorHandle( iX, iY )
{

}

uint32
FVectorHandleSegment::GetType()
{
    return FVectorPoint::POINT_TYPE_HANDLE_SEGMENT;
}

void
FVectorHandleSegment::SetParent(FVectorSegment* iParentSegment)
{
    mParentSegment = iParentSegment;
}

FVectorSegment*
FVectorHandleSegment::GetParent()
{
    return mParentSegment;
}
