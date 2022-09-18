#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

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

    iBLContext.strokeRoundRect( 0.0f, 0.0f, mWidth, mHeight, 0.0f, 0.0f );
}
