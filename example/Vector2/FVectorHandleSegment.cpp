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
    return HANDLE_TYPE_SEGMENT;
}
