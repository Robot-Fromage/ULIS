#include <blend2d.h>
#include "Vector/Vector.h"

FVectorCircle::~FVectorCircle()
{
}

FVectorCircle::FVectorCircle()
    : FVectorObject("Circle")
{
    mRadius = 0.0f;
}

FVectorCircle::FVectorCircle( double iRadius )
    : FVectorObject("Circle")
{
    mRadius = iRadius;
}

FVectorObject*
FVectorCircle::CopyShape()
{
    FVectorCircle* circleCopy = new FVectorCircle ( mRadius );

    return static_cast<FVectorObject*>( circleCopy );
}

void
FVectorCircle::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    if ( iRoi.Area() != 0.0f )
    {
        FRectD clip = ( iRoi & mBBox );

        if ( clip.Area() )
        {
            iBLContext.clipToRect ( iRoi.x, iRoi.y, iRoi.w, iRoi.h );
        }
    }

    iBLContext.setCompOp( BL_COMP_OP_SRC_COPY );

    if( mIsFilled )
    {
        iBLContext.setFillStyle( BLRgba32( mFillColor ) );
        iBLContext.fillCircle( 0.0f, 0.0f, mRadius );
    }

    iBLContext.setStrokeStyle( BLRgba32( mStrokeColor ) );
    iBLContext.setStrokeWidth( mStrokeWidth );
    iBLContext.strokeCircle  ( 0.0f, 0.0f, mRadius );
}

FVectorObject*
FVectorCircle::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    if( FMath::Sqrt((iX*iX) + (iY*iY)) <= mRadius )
    {
        return this;
    }

    return nullptr;
}

void
FVectorCircle::SetRadius( double iRadius )
{
    mRadius = iRadius;

    mBBox.x = -mRadius - mStrokeWidth;
    mBBox.y = -mRadius - mStrokeWidth;
    mBBox.w =  ( mRadius +  mStrokeWidth ) * 2;
    mBBox.h =  ( mRadius +  mStrokeWidth ) * 2;
}

double
FVectorCircle::GetRadius()
{
    return mRadius;
}
