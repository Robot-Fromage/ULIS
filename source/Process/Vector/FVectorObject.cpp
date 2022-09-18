#include <ULIS>
#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

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

void
FVectorObject::UpdateMatrix( BLContext& iBLContext )
{
    iBLContext.save();
    iBLContext.resetMatrix();
    iBLContext.translate( mTranslation.x,mTranslation.y );
    iBLContext.rotate( mRotation );
    iBLContext.scale( mScaling.x,mScaling.y );
    mLocalMatrix = iBLContext.userMatrix();
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
FVectorObject::AddChild( FVectorObject* iChild )
{
    mChildrenList.push_back( iChild );
}

ULIS_NAMESPACE_END
