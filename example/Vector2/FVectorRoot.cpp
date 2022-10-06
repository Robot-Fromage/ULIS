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
FVectorRoot::ClearSelection()
{
    for( std::list<FVectorObject*>::iterator it = mSelectedObjectList.begin(); it != mSelectedObjectList.end(); ++it )
    {
        FVectorObject *obj = (*it);

        obj->SetIsSelected ( false );
    }

    mSelectedObjectList.clear();
}

void
FVectorRoot::Select( BLContext& iBLContext, FVectorObject& iVecObj )
{
    iVecObj.SetIsSelected( true );

    mSelectedObjectList.push_back( &iVecObj );
}

void
FVectorRoot::Select( BLContext& iBLContext, double iX, double iY, double iRadius )
{
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
        Select ( iBLContext, iChild );
    }

    for( std::list<FVectorObject*>::iterator it = iChild.GetChildrenList().begin(); it != iChild.GetChildrenList().end(); ++it )
    {
        FVectorObject *obj = (*it);

        RecursiveSelect( iBLContext, *obj, localCoords.x, localCoords.y, localRadius );
    }
}
