#include <blend2d.h>
#include "Vector/Vector.h"

FVectorRoot::~FVectorRoot()
{
}

FVectorRoot::FVectorRoot()
    : FVectorObject()
{
}

FVectorRoot::FVectorRoot( std::string iName )
    : FVectorObject( iName )
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
    FVectorObject* pickedObject = RecursiveSelect( iBLContext, *this, iX, iY, iRadius );

    if ( pickedObject )
    {
        if( typeid ( *pickedObject ) == typeid ( FVectorPathLoop ) )
        {
            pickedObject = pickedObject->GetParent();
        }

        Select ( iBLContext, *pickedObject );
    }
}

void
FVectorRoot::Bucket( BLContext& iBLContext, double iX, double iY, uint32 iFillColor )
{
    FVectorObject* pickedObject = RecursiveSelect( iBLContext, *this, iX, iY, 1.0f );

    if ( pickedObject )
    {
        pickedObject->SetFilled(true);
        pickedObject->SetFillColor( iFillColor );
    }
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

FVectorObject*
FVectorRoot::RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double iX, double iY, double iRadius )
{
    BLMatrix2D inverseLocalMatrix;
    BLPoint localCoords;
    BLPoint localSize;
    double localRadius;
    FVectorObject* pickedObject = nullptr;

    BLMatrix2D::invert( inverseLocalMatrix, iChild.GetLocalMatrix() );

    localCoords = inverseLocalMatrix.mapPoint( iX, iY );
    localSize   = inverseLocalMatrix.mapPoint( iX + iRadius, 0.0f );

    localRadius = localSize.x - localCoords.x;

    for( std::list<FVectorObject*>::iterator it = iChild.GetChildrenList().begin(); it != iChild.GetChildrenList().end(); ++it )
    {
        FVectorObject *obj = (*it);

        pickedObject = RecursiveSelect( iBLContext, *obj, localCoords.x, localCoords.y, localRadius );

        if ( pickedObject )
        {
            return pickedObject;
        }
    }

    return iChild.Pick( iBLContext, localCoords.x, localCoords.y, localRadius );
}
