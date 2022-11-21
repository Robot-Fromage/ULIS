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
    if ( std::find( mSelectedObjectList.begin(), mSelectedObjectList.end(), &iVecObj ) == mSelectedObjectList.end())
    {
        iVecObj.SetIsSelected( true );

        mSelectedObjectList.push_back( &iVecObj );
    }
}

FVectorObject*
FVectorRoot::CopyShape()
{
    return new FVectorRoot();
}

void
FVectorRoot::Select( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    FVectorObject* pickedObject = RecursiveSelect( iBLContext, *this, iX, iY, iRadius );

    if ( pickedObject )
    {
        if( typeid ( *pickedObject ) == typeid ( FVectorLoop ) )
        {
            pickedObject = pickedObject->GetParent();
        }

        Select ( iBLContext, *pickedObject );
    }
}

FVectorGroup*
FVectorRoot::GroupSelectdObjects( )
{
    FVectorGroup* group = new FVectorGroup();
    BLPoint averageTranslation = { 0.0f, 0.0f };
    BLContext& blctx = FVectorEngine::GetBLContext();

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

    AddChild ( group );

    group->Translate( averageTranslation.x, averageTranslation.y );
    group->UpdateMatrix( blctx );

    for( std::list<FVectorObject*>::iterator it = mSelectedObjectList.begin(); it != mSelectedObjectList.end(); ++it )
    {
        FVectorObject *obj = (*it);

        obj->GetParent()->RemoveChild( obj );

        group->AddChild( obj );
    }

    group->UpdateShape();


    return group;
}

void
FVectorRoot::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    for( std::list<FVectorObject*>::iterator it = mSelectedObjectList.begin(); it != mSelectedObjectList.end(); ++it )
    {
        FVectorObject *obj = (*it);
        FRectD bbox = obj->GetBBox( false );

        iBLContext.save();
        iBLContext.setMatrix( obj->GetWorldMatrix() );
        iBLContext.strokeRect( bbox.x, bbox.y, bbox.w, bbox.h );
        iBLContext.restore();
    }
}

void
FVectorRoot::InvalidateObject( FVectorObject* iObject )
{
    mInvalidatedObjectList.push_back( iObject );
}

void
FVectorRoot::Update()
{
    for( std::list<FVectorObject*>::iterator it = mInvalidatedObjectList.begin(); it != mInvalidatedObjectList.end(); ++it )
    {
        FVectorObject *obj = (*it);

        obj->UpdateShape();
    }

    mInvalidatedObjectList.clear();
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
FVectorRoot::RecursiveSelect( BLContext& iBLContext, FVectorObject& iObj, double iX, double iY, double iRadius )
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

        pickedChild = RecursiveSelect( iBLContext, *child, localCoords.x, localCoords.y, localRadius );

        if ( pickedChild )
        {
            pickedObject = pickedChild;
        }
    }

    return ( pickedObject ) ? pickedObject : iObj.Pick( iBLContext, localCoords.x, localCoords.y, localRadius );
}
