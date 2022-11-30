#include <blend2d.h>
#include "Vector/Vector.h"

FVectorEngine::~FVectorEngine()
{
    GetBLContext().end();
}

FVectorEngine::FVectorEngine( double iWidth, double iHeight )
    : mScene ( "Vector Scene" )
{
    mBLImage = new BLImage( iWidth, iHeight, BL_FORMAT_PRGB32 );

    GetBLContext().begin( *mBLImage );
}

BLImage&
FVectorEngine::GetBLImage()
{
    return *mBLImage;
}

BLContext&
FVectorEngine::GetBLContext()
{
    static BLContext* blctx;

    if ( blctx == nullptr )
    {
        blctx = new BLContext();
    }

    return *blctx;
}

FRectD&
FVectorEngine::GetInvalidateRegion()
{
    return mRoi;
}

void
FVectorEngine::Render( FBlock& iBlock )
{
    BLContext& blctx = FVectorEngine::GetBLContext();
    static FRectD zeroRectangle;
    // Blend2D part
   /* BLContextCreateInfo createInfo{};*/

    // Configure the number of threads to use.
    /*createInfo.threadCount = 1;*/

    blctx.setFillStyle(BLRgba32(0xFFFFFFFF));

    if ( mRoi != zeroRectangle )
    {
        blctx.fillRect( mRoi.x, mRoi.y, mRoi.w, mRoi.h );

        blctx.setStrokeStyle(BLRgba32(0xFFFF0000));
        blctx.setStrokeWidth(1.0f);
        blctx.strokeRect( mRoi.x, mRoi.y, mRoi.w, mRoi.h );

    }
    else
    {
        blctx.fillAll();
    }

    mScene.Draw( mRoi, 0 );

    // Reset region of interest after each draw
    memset ( &mRoi, 0, sizeof ( mRoi ) );

    blctx.flush( BL_CONTEXT_FLUSH_SYNC );
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

void
FVectorEngine::InvalidateRegion( FRectD& iRegion )
{
    mRoi.x = iRegion.x;
    mRoi.y = iRegion.y;
    mRoi.w = iRegion.w;
    mRoi.h = iRegion.h;
}
