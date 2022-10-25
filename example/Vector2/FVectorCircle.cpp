#include <blend2d.h>
#include "Vector/Vector.h"

FVectorCircle::~FVectorCircle()
{
}

FVectorCircle::FVectorCircle()
    : FVectorObject()
{
    mRadius = 0.0f;
}

FVectorCircle::FVectorCircle( double iRadius )
    : FVectorObject()
{
    mRadius = iRadius;
}

void
FVectorCircle::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{


    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    iBLContext.setStrokeStyle( BLRgba32( mStrokeColor ) );
    iBLContext.setStrokeWidth( mStrokeWidth );

    if ( iRoi.Area() != 0.0f )
    {
        FRectD clip = ( iRoi & mBBox );

        if ( clip.Area() )
        {
            iBLContext.clipToRect ( iRoi.x, iRoi.y, iRoi.w, iRoi.h );
        }
    }

    iBLContext.strokeCircle( 0.0f, 0.0f, mRadius );

    if( mIsFilled )
    {
        iBLContext.setFillStyle( BLRgba32( mFillColor ) );
        iBLContext.fillCircle( 0.0f, 0.0f, mRadius );
    }
}

bool
FVectorCircle::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    if( FMath::Sqrt((iX*iX) + (iY*iY)) <= mRadius )
    {
        return true;
    }

    return false;
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
