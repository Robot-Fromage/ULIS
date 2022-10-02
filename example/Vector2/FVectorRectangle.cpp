#include <blend2d.h>
#include "Vector/Vector.h"

FVectorRectangle::~FVectorRectangle()
{
}

FVectorRectangle::FVectorRectangle()
    : FVectorObject()
    , mWidth( 0 )
    , mHeight( 0 )
{
}

FVectorRectangle::FVectorRectangle( double iWidth, double iHeight )
    : FVectorObject()
    , mWidth( iWidth )
    , mHeight( iHeight )
{
}

void
FVectorRectangle::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{
    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));

    iBLContext.strokeRoundRect( -mWidth * 0.5f, -mHeight * 0.5f, mWidth, mHeight, 0.0f, 0.0f );
}

bool
FVectorRectangle::PickShape( BLContext& iBLContext, double iX, double iY )
{
    double x1 = -mWidth  * 0.5f;
    double y1 = -mHeight * 0.5f;
    double x2 = x1 + mWidth;
    double y2 = y1 + mHeight;

    if( ( iX >= x1 ) && ( iX <= x2 ) &&
        ( iY >= y1 ) && ( iY <= y2 ) )
    {
        return true;
    }

    return false;
}

void
FVectorRectangle::SetSize( double iWidth, double iHeight )
{
    mWidth  = iWidth;
    mHeight = iHeight;
}

double FVectorRectangle::GetWidth()
{
    return mWidth;
}

double FVectorRectangle::GetHeight()
{
    return mHeight;
}
