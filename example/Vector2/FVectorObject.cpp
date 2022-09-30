#include <blend2d.h>
#include "Vector/Vector.h"

FVectorObject::~FVectorObject()
{
}

FVectorObject::FVectorObject()
    : mTranslation( 0, 0 )
    , mRotation( 0 )
    , mScaling ( 1.0f, 1.0f )
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
    iBLContext.transform( mLocalMatrix );
    iBLContext.restore();
    mWorldMatrix = iBLContext.userMatrix();
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
FVectorObject::PickShape( BLContext& iBLContext, double iX, double iY )
{
    return false;
}

bool
FVectorObject::Pick( BLContext& iBLContext, double iX, double iY )
{
    return PickShape( iBLContext, iX, iY );
}

void
FVectorObject::AddChild( FVectorObject* iChild )
{
    mChildrenList.push_back( iChild );
}

void
FVectorObject::RemoveChild( FVectorObject* iChild )
{
    mChildrenList.remove(iChild);
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
