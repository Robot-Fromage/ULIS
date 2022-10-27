#include <blend2d.h>
#include "Vector/Vector.h"

FVectorObject::~FVectorObject()
{
}

FVectorObject::FVectorObject()
    : mTranslation( 0, 0 )
    , mRotation( 0 )
    , mScaling ( 1.0f, 1.0f )
    , mStrokeColor ( 0xFF000000 )
    , mFillColor ( 0xFF000000 )
    , mStrokeWidth ( 4.0f )
    , mParent ( nullptr )
    , mIsFilled ( false )
    , mIsSelected ( false )
{
    mLocalMatrix.reset();
    mInverseLocalMatrix.reset();
    mWorldMatrix.reset();
    mInverseWorldMatrix.reset();
}

FVectorObject::FVectorObject( std::string iName )
    : FVectorObject( )
{
    mName.assign( iName );
}

void
FVectorObject::SetIsSelected( bool iIsSelected )
{
    mIsSelected = iIsSelected;
}

void
FVectorObject::Translate( double iX, double iY )
{
    mTranslation.x = iX;
    mTranslation.y = iY;
}

void
FVectorObject::Rotate( double iAngle )
{
    mRotation = iAngle;
}

void
FVectorObject::Scale( double iX, double iY )
{
    mScaling.x = iX;
    mScaling.y = iY;
}

double
FVectorObject::GetScalingX()
{
    return mScaling.x;
}

double
FVectorObject::GetScalingY()
{
    return mScaling.y;
}

double
FVectorObject::GetRotation()
{
    return mRotation;
}

double
FVectorObject::GetTranslationX()
{
    return mTranslation.x;
}

double
FVectorObject::GetTranslationY()
{
    return mTranslation.y;
}

void
FVectorObject::UpdateMatrix( BLContext& iBLContext )
{
    iBLContext.save();
    iBLContext.resetMatrix();
    iBLContext.translate( mTranslation.x, mTranslation.y );
    iBLContext.rotate( mRotation );
    iBLContext.scale( mScaling.x, mScaling.y );
    mLocalMatrix = iBLContext.userMatrix();
    /*iBLContext.restore();

    iBLContext.save();*/

    BLMatrix2D::invert( mInverseLocalMatrix, mLocalMatrix );

    if( mParent)
    {
        iBLContext.setMatrix( mParent->mWorldMatrix );
        iBLContext.transform( mLocalMatrix );
        mWorldMatrix = iBLContext.userMatrix();

        BLMatrix2D::invert( mInverseWorldMatrix, mWorldMatrix );
    }
     else
    {
        memcpy( &mWorldMatrix       , &mLocalMatrix       , sizeof ( mLocalMatrix        ) );
        memcpy( &mInverseWorldMatrix, &mInverseLocalMatrix, sizeof ( mInverseLocalMatrix ) );
    }

    // recurse
    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);

        child->UpdateMatrix( iBLContext );
    }

    iBLContext.restore();
}

FRectD
FVectorObject::GetBBox( bool iWorld )
{

    if ( iWorld == true )
    {
        BLPoint origin = mWorldMatrix.mapPoint( mBBox.x, mBBox.y );
        BLPoint size = mWorldMatrix.mapVector( mBBox.w, mBBox.h );
        FRectD worldBBox = { origin.x, origin.y, size.x, size.y };
        
        return worldBBox;
    }
    

    return mBBox;
}

void
FVectorObject::DrawChildren( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);

        child->Draw( iBlock, iBLContext, iRoi );
    }
}

void
FVectorObject::Draw( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    FRectD localRoi = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Adapt the Region-Of-Interest to the local coordinates
    if( iRoi.Area() != 0.0f )
    {
        BLPoint roiCornerOrigin = { iRoi.x, iRoi.y };
        BLPoint roiCornerSize = { iRoi.w, iRoi.h };
        BLPoint localRoiCornerOrigin = mInverseLocalMatrix.mapPoint( roiCornerOrigin.x, roiCornerOrigin.y );
        BLPoint localRoiCornerSize = mInverseLocalMatrix.mapVector( roiCornerSize.x, roiCornerSize.y );

        localRoi.x = localRoiCornerOrigin.x;
        localRoi.y = localRoiCornerOrigin.y;
        localRoi.w = localRoiCornerSize.x;
        localRoi.h = localRoiCornerSize.y;
    }

            /*printf("%s : %f %f %f %f\n",mName.c_str(), iRoi.x,iRoi.y,iRoi.w,iRoi.h);*/

    iBLContext.save();
    iBLContext.transform( mLocalMatrix );
    /*iBLContext.translate( mTranslation.x, mTranslation.y );*/
    DrawShape( iBlock, iBLContext, localRoi );

    DrawChildren( iBlock, iBLContext, localRoi );

    iBLContext.restore();
}

void
FVectorObject::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
}

bool
FVectorObject::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    return false;
}

bool
FVectorObject::IsFilled()
{
    return mIsFilled;
}

void
FVectorObject::MoveBack()
{
    if ( mParent )
    {
        mParent->
    }
}

FVec2D
FVectorObject::WorldCoordinatesToLocal( double iX, double iY )
{
    BLPoint localCoords;
    BLPoint localSize;
    FVec2D localPoint;

    localCoords = mInverseWorldMatrix.mapPoint( iX, iY );

    localPoint.x = localCoords.x;
    localPoint.y = localCoords.y;

    return localPoint;
}

FVectorObject*
FVectorObject::GetParent()
{
    return mParent;
}

bool
FVectorObject::Pick( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    return PickShape( iBLContext, iX, iY, iRadius );
}

void
FVectorObject::AddChild( FVectorObject* iChild )
{
    iChild->mParent = this;

    mChildrenList.push_back( iChild );
}

void
FVectorObject::RemoveChild( FVectorObject* iChild )
{
    iChild->mParent = nullptr;

    mChildrenList.remove(iChild);
}

void
FVectorObject::SetStrokeColor( uint32 iColor )
{
    mStrokeColor = iColor;
}

void
FVectorObject::SetFillColor( uint32 iColor )
{
    mFillColor = iColor;
}

void
FVectorObject::SetFilled( bool iIsFilled)
{
    mIsFilled = iIsFilled;
}

void
FVectorObject::SetStrokeWidth( double iWidth )
{
    mStrokeWidth = iWidth;
}

double
FVectorObject::GetStrokeWidth()
{
    return mStrokeWidth;
}

void 
FVectorObject::CopyTransformation( FVectorObject& iObject )
{
    iObject.mRotation    = mRotation;
    iObject.mScaling     = mScaling;
    iObject.mTranslation = mTranslation;
}

std::list<FVectorObject*>&
FVectorObject::GetChildrenList()
{
    return mChildrenList;
}

BLMatrix2D&
FVectorObject::GetLocalMatrix()
{
    return mLocalMatrix;
}

BLMatrix2D&
FVectorObject::GetWorldMatrix()
{
    return mWorldMatrix;
}
