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
    , mParent( nullptr )
    , mIsFilled( false )
    , mIsSelected( false )
{
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

    if( mParent)
    {
        iBLContext.setMatrix( mParent->mWorldMatrix );
        iBLContext.transform( mLocalMatrix );
        mWorldMatrix = iBLContext.userMatrix();
    }
     else
    {
        memcpy( &mWorldMatrix, &mLocalMatrix, sizeof ( mLocalMatrix ) );
    }

    BLMatrix2D::invert( mInverseWorldMatrix, mWorldMatrix );

    // recurse
    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);

        child->UpdateMatrix( iBLContext );
    }

    iBLContext.restore();
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
    static FRectD zeroRectangle = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Adapt the Region-Of-Interest to the local coordinates
    if( iRoi != zeroRectangle )
    {
        BLPoint roiCornerTop = { iRoi.x, iRoi.y };
        BLPoint roiCornerBottom = { iRoi.x + iRoi.w, iRoi.y + iRoi.h };
        BLPoint localRoiCornerTop = mInverseWorldMatrix.mapPoint( roiCornerTop.x, roiCornerTop.y );
        BLPoint localRoiCornerBottom = mInverseWorldMatrix.mapPoint( roiCornerBottom.x, roiCornerBottom.y );

        iRoi.x = localRoiCornerTop.x;
        iRoi.y = localRoiCornerTop.y;
        iRoi.w = localRoiCornerBottom.x - localRoiCornerTop.x;
        iRoi.h = localRoiCornerBottom.y - localRoiCornerTop.y;
    }

    iBLContext.save();
    iBLContext.transform( mLocalMatrix );
    /*iBLContext.translate( mTranslation.x, mTranslation.y );*/
    DrawShape( iBlock, iBLContext, iRoi );

    DrawChildren( iBlock, iBLContext, iRoi );

    // Restore the ROI to the parent coordinates. At this step it may have been modified by some child
    if( iRoi != zeroRectangle )
    {
        BLPoint roiCornerTop = { iRoi.x, iRoi.y };
        BLPoint roiCornerBottom = { iRoi.x + iRoi.w, iRoi.y + iRoi.h };
        BLPoint parentRoiCornerTop = mLocalMatrix.mapPoint( roiCornerTop.x, roiCornerTop.y );
        BLPoint parentRoiCornerBottom = mLocalMatrix.mapPoint( roiCornerBottom.x, roiCornerBottom.y );

        iRoi.x = parentRoiCornerTop.x;
        iRoi.y = parentRoiCornerTop.y;
        iRoi.w = parentRoiCornerBottom.x - parentRoiCornerTop.x;
        iRoi.h = parentRoiCornerBottom.y - parentRoiCornerTop.y;
    }

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
