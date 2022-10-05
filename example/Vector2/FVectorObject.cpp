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
{
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
    iBLContext.restore();

    iBLContext.save();

    if( mParent)
    {
        iBLContext.setMatrix( mParent->mWorldMatrix );
        iBLContext.transform( mLocalMatrix );
        mWorldMatrix = iBLContext.userMatrix();
    }
     else
    {
        iBLContext.resetMatrix();
        mWorldMatrix = iBLContext.userMatrix();
    }

    iBLContext.restore();

}

void
FVectorObject::DrawChildren( FBlock& iBlock,BLContext& iBLContext )
{
    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);

        child->Draw( iBlock, iBLContext );
    }
}

void
FVectorObject::Draw( FBlock& iBlock, BLContext& iBLContext )
{
    iBLContext.save();
    iBLContext.transform( mLocalMatrix );
    /*iBLContext.translate( mTranslation.x, mTranslation.y );*/
    DrawShape( iBlock, iBLContext );

    DrawChildren( iBlock, iBLContext );

    iBLContext.restore();
}

void
FVectorObject::DrawShape( FBlock& iBlock, BLContext& iBLContext )
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
    BLMatrix2D inverseWorldMatrix;
    BLPoint localCoords;
    BLPoint localSize;
    FVec2D localPoint;

    BLMatrix2D::invert( inverseWorldMatrix, mWorldMatrix );

    localCoords = inverseWorldMatrix.mapPoint( iX, iY );

    localPoint.x = localCoords.x;
    localPoint.y = localCoords.y;

    return localPoint;
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
