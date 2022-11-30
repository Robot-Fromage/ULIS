#include <blend2d.h>
#include "Vector/Vector.h"

FVectorRoot::~FVectorRoot()
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
FVectorRoot::Select( FVectorObject& iVecObj )
{
    if ( std::find( mSelectedObjectList.begin(), mSelectedObjectList.end(), &iVecObj ) == mSelectedObjectList.end())
    {
        iVecObj.SetIsSelected( true );

        mSelectedObjectList.push_back( &iVecObj );
    }
}

FVectorObject*
FVectorRoot::CopyShape()
{
    return new FVectorRoot( mName );
}

void
FVectorRoot::Select( double iX, double iY, double iRadius )
{
    FVectorObject* pickedObject = RecursiveSelect( *this, iX, iY, iRadius );

    if ( pickedObject )
    {
        if( typeid ( *pickedObject ) == typeid ( FVectorLoop ) )
        {
            pickedObject = pickedObject->GetParent();
        }

        Select ( *pickedObject );
    }
}

FVectorGroup*
FVectorRoot::GroupSelectdObjects( )
{
    FVectorGroup* group = new FVectorGroup();
    BLPoint averageTranslation = { 0.0f, 0.0f };
    BLContext& blctx = FVectorEngine::GetBLContext();
    // We don't use the mSelectedObjectList because we want to keep the same order
    // and we work on a copy to be able to delete the objects while iterating
    std::list<FVectorObject*> objectList = mChildrenList;

    if ( mSelectedObjectList.size() )
    {
        for( std::list<FVectorObject*>::iterator it = mSelectedObjectList.begin(); it != mSelectedObjectList.end(); ++it )
        {
            FVectorObject *obj = (*it);
            BLPoint origin = obj->GetWorldMatrix().mapPoint( 0.0f, 0.0f );

            averageTranslation.x += origin.x;
            averageTranslation.y += origin.y;
        }

        averageTranslation.x /= mSelectedObjectList.size();
        averageTranslation.y /= mSelectedObjectList.size();

        averageTranslation = this->GetInverseWorldMatrix().mapPoint ( averageTranslation.x, averageTranslation.y );
    }

    AppendChild ( group );

    group->Translate( averageTranslation.x, averageTranslation.y );
    group->UpdateMatrix();

    while( objectList.size () )
    {
        FVectorObject *obj = objectList.back();

        if ( obj->IsSelected() == true )
        {
            obj->GetParent()->RemoveChild( obj );

            group->PrependChild( obj );
        }

        objectList.pop_back();
    }

    group->Update();


    return group;
}

void
FVectorRoot::DrawShape( FRectD& iRoi, uint64 iFlags )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    for( std::list<FVectorObject*>::iterator it = mSelectedObjectList.begin(); it != mSelectedObjectList.end(); ++it )
    {
        FVectorObject *obj = (*it);
        FRectD bbox = obj->GetBBox( false );

        blctx.save();
        blctx.setMatrix( obj->GetWorldMatrix() );
        blctx.strokeRect( bbox.x, bbox.y, bbox.w, bbox.h );
        blctx.restore();
    }
}

void
FVectorRoot::InvalidateObject( FVectorObject* iObject )
{
    mInvalidatedObjectList.push_back( iObject );
}

void
FVectorRoot::UpdateShape()
{
    for( std::list<FVectorObject*>::iterator it = mInvalidatedObjectList.begin(); it != mInvalidatedObjectList.end(); ++it )
    {
        FVectorObject *obj = (*it);

        obj->Update();
    }

    mInvalidatedObjectList.clear();
}

void
FVectorRoot::Bucket( double iX, double iY, uint32 iFillColor )
{
    FVectorObject* pickedObject = RecursiveSelect( *this, iX, iY, 1.0f );

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
FVectorRoot::RecursiveSelect( FVectorObject& iObj, double iX, double iY, double iRadius )
{
    BLMatrix2D inverseLocalMatrix;
    BLPoint localCoords;
    BLPoint localSize;
    double localRadius;
    FVectorObject* pickedObject = nullptr;

    BLMatrix2D::invert( inverseLocalMatrix, iObj.GetLocalMatrix() );

    localCoords = inverseLocalMatrix.mapPoint( iX, iY );
    localSize   = inverseLocalMatrix.mapPoint( iX + iRadius, 0.0f );

    localRadius = localSize.x - localCoords.x;

    for( std::list<FVectorObject*>::iterator it = iObj.GetChildrenList().begin(); it != iObj.GetChildrenList().end(); ++it )
    {
        FVectorObject *child = (*it);
        FVectorObject* pickedChild = nullptr;

        pickedChild = RecursiveSelect( *child, localCoords.x, localCoords.y, localRadius );

        if ( pickedChild )
        {
            pickedObject = pickedChild;
        }
    }

    return ( pickedObject ) ? pickedObject : iObj.Pick( localCoords.x, localCoords.y, localRadius );
}
