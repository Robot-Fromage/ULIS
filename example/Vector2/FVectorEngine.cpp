#include <blend2d.h>
#include "Vector/Vector.h"

FVectorEngine::~FVectorEngine()
{
}

FVectorEngine::FVectorEngine()
{

}

void
FVectorEngine::Draw( FBlock& iBlock, BLContext& iBLContext )
{
    static FRectD zeroRectangle;

    if ( mRoi != zeroRectangle )
    {
        iBLContext.fillRect( mRoi.x, mRoi.y, mRoi.w, mRoi.h );

        /*iBLContext.setStrokeStyle(BLRgba32(0xFFFF0000));
        iBLContext.setStrokeWidth(1.0f);
        iBLContext.strokeRect( mRoi.x, mRoi.y, mRoi.w, mRoi.h );*/
    }
    else
    {
        iBLContext.fillAll();
    }



    mScene.Draw( iBlock, iBLContext, mRoi );

    // Reset region of interest after each draw
    memset ( &mRoi, 0, sizeof ( mRoi ) );
}

FVectorRoot&
FVectorEngine::GetScene()
{
    return mScene;
}

void
FVectorEngine::InvalidateRegion( double x, double y, double w, double h )
{
    mRoi.x = x;
    mRoi.y = y;
    mRoi.w = w;
    mRoi.h = h;
}
