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
FVectorRoot::Select( BLContext& iBLContext, double x, double y )
{
    mSelectedObjectList.clear();

    RecursiveSelect( iBLContext, *this, x, y );
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
FVectorRoot::RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double iX, double iY )
{
    BLMatrix2D inverseLocalMatrix;
    BLPoint localCoords;

    BLMatrix2D::invert( inverseLocalMatrix, iChild.GetLocalMatrix() );

    localCoords = inverseLocalMatrix.mapPoint( iX, iY);

    if( iChild.Pick( iBLContext, localCoords.x, localCoords.y ) )
    {
        mSelectedObjectList.push_back( &iChild );
    }

    for( std::list<FVectorObject*>::iterator it = iChild.GetChildrenList().begin(); it != iChild.GetChildrenList().end(); ++it )
    {
        FVectorObject *obj = (*it);

        RecursiveSelect( iBLContext, *obj, localCoords.x, localCoords.y );
    }
}
