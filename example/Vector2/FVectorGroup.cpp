#include <blend2d.h>
#include "Vector/Vector.h"

FVectorGroup::~FVectorGroup()
{
}

FVectorGroup::FVectorGroup()
    : FVectorObject()
{
}

FVectorObject*
FVectorGroup::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    if ( ( iX > mBBox.x ) && ( iX < ( mBBox.x + mBBox.w ) )
      && ( iY > mBBox.y ) && ( iY < ( mBBox.y + mBBox.h ) ) )
    {
        return this;
    }

    return nullptr;
}

FVectorObject*
FVectorGroup::CopyShape()
{
    return new FVectorGroup();
}

void
FVectorGroup::UpdateShape()
{
    FRectD bbox;
    int init = 0;

    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);
        FRectD childBBox = child->GetBBox( true );

        bbox = ( init == 0 ) ? childBBox : bbox | childBBox;

        init = 1;
    }

    BLPoint localOrigin = mInverseWorldMatrix.mapPoint( bbox.x, bbox.y );
    BLPoint LocalSize = mInverseWorldMatrix.mapVector( bbox.w, bbox.h );

    mBBox.x = localOrigin.x;
    mBBox.y = localOrigin.y;
    mBBox.w = LocalSize.x;
    mBBox.h = LocalSize.y;
}
