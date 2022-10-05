#include <blend2d.h>
#include "Vector/Vector.h"

FVectorRoot::~FVectorRoot()
{
}

FVectorRoot::FVectorRoot()
    : FVectorObject()
{
}

void
FVectorRoot::Select( BLContext& iBLContext, FVectorObject& iVecObj )
{
    mSelectedObjectList.clear();

    mSelectedObjectList.push_back(&iVecObj);
}

void
FVectorRoot::Select( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    mSelectedObjectList.clear();

    RecursiveSelect( iBLContext, *this, iX, iY, iRadius );
}

FVectorObject*
FVectorRoot::GetLastSelected()
{
    if ( mSelectedObjectList.empty() == true )
    {
        return nullptr;
    }

    return mSelectedObjectList.back();
}

void
FVectorRoot::RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double iX, double iY, double iRadius )
{
    BLMatrix2D inverseLocalMatrix;
    BLPoint localCoords;
    BLPoint localSize;
    double localRadius;

    BLMatrix2D::invert( inverseLocalMatrix, iChild.GetLocalMatrix() );

    localCoords = inverseLocalMatrix.mapPoint( iX, iY );
    localSize   = inverseLocalMatrix.mapPoint( iX + iRadius, 0.0f );

    localRadius = localSize.x - localCoords.x;

    if( iChild.Pick( iBLContext, localCoords.x, localCoords.y, localRadius ) )
    {
        mSelectedObjectList.push_back( &iChild );
    }

    for( std::list<FVectorObject*>::iterator it = iChild.GetChildrenList().begin(); it != iChild.GetChildrenList().end(); ++it )
    {
        FVectorObject *obj = (*it);

        RecursiveSelect( iBLContext, *obj, localCoords.x, localCoords.y, localRadius );
    }
}
