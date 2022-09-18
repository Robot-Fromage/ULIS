#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

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
FVectorCircle::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{
    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));

    iBLContext.strokeCircle( 0.0f, 0.0f, mRadius );
}
