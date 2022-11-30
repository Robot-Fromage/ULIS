#include <blend2d.h>
#include "Vector/Vector.h"

FVectorRectangle::~FVectorRectangle()
{
}

FVectorRectangle::FVectorRectangle( std::string iName )
    : FVectorObject( iName )
    , mWidth( 0 )
    , mHeight( 0 )
{
}

FVectorRectangle::FVectorRectangle( std::string iName, double iWidth, double iHeight )
    : FVectorObject( iName )
    , mWidth( iWidth )
    , mHeight( iHeight )
{
}

FVectorObject*
FVectorRectangle::CopyShape()
{
    FVectorRectangle* rectangleCopy = new FVectorRectangle ( mName, mWidth, mHeight );

    return static_cast<FVectorObject*>( rectangleCopy );
}

void
FVectorRectangle::DrawShape( FRectD &iRoi, uint64 iFlags )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    blctx.setCompOp(BL_COMP_OP_SRC_COPY);

    if( mIsFilled )
    {
        blctx.setFillStyle( BLRgba32( mFillColor ) );
        blctx.fillRoundRect( -mWidth * 0.5f, -mHeight * 0.5f, mWidth, mHeight, 0.0f, 0.0f );
    }

    blctx.setStrokeStyle ( BLRgba32( mStrokeColor ) );
    blctx.setStrokeWidth ( mStrokeWidth );
    blctx.strokeRoundRect( -mWidth * 0.5f, -mHeight * 0.5f, mWidth, mHeight, 0.0f, 0.0f );
}

FVectorObject*
FVectorRectangle::PickShape( double iX, double iY, double iRadius )
{
    double x1 = - mWidth  * 0.5f;
    double y1 = - mHeight * 0.5f;
    double x2 = x1 + mWidth;
    double y2 = y1 + mHeight;

    if( ( iX >= x1 ) && ( iX <= x2 ) &&
        ( iY >= y1 ) && ( iY <= y2 ) )
    {
        return this;
    }

    return nullptr;
}

void
FVectorRectangle::SetSize( double iWidth, double iHeight )
{
    mWidth  = iWidth;
    mHeight = iHeight;

    mBBox.x = (-mWidth * 0.5f ) - mStrokeWidth;
    mBBox.y = (-mHeight * 0.5f ) - mStrokeWidth;
    mBBox.w =  ( ( mWidth * 0.5f ) +  mStrokeWidth ) * 2;
    mBBox.h =  ( ( mHeight * 0.5f ) +  mStrokeWidth ) * 2;
}

double FVectorRectangle::GetWidth()
{
    return mWidth;
}

double FVectorRectangle::GetHeight()
{
    return mHeight;
}
