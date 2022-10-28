#include <blend2d.h>
#include "Vector/Vector.h"

FVectorHandlePoint::~FVectorHandlePoint()
{
}

FVectorHandlePoint::FVectorHandlePoint()
    : FVectorHandle()
{
    SetParent( nullptr );
}

FVectorHandlePoint::FVectorHandlePoint( FVectorPoint* iParentPoint )
    : FVectorHandle()
{
   SetParent( iParentPoint );
}

void
FVectorHandlePoint::SetParent( FVectorPoint* iParentPoint )
{
    mParentPoint = iParentPoint;
}

FVectorPoint*
FVectorHandlePoint::GetParent()
{
    return mParentPoint;
}

uint32
FVectorHandlePoint::GetType()
{
    return FVectorPoint::POINT_TYPE_HANDLE_POINT;
}
