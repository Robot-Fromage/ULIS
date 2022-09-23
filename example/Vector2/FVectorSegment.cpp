#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegment::~FVectorSegment()
{
}

FVectorSegment::FVectorSegment()
{
}

FVectorSegment::FVectorSegment( FVectorPoint* iPoint0, FVectorPoint* iPoint1 )
{
    mPoint[0] = iPoint0;
    mPoint[1] = iPoint1;

    mPoint[0]->AddSegment( this );
    mPoint[1]->AddSegment( this );
}

FVectorPoint* FVectorSegment::GetPoint( int iPointNum )
{
    return mPoint[iPointNum];
}

void
FVectorSegment::Draw( FBlock& iBlock
                    , BLContext& iBLContext )
{
    BLPath path;

    BLPoint point0;
    BLPoint point1;

    point0.x = mPoint[0]->GetX();
    point0.y = mPoint[0]->GetY();

    point1.x = mPoint[1]->GetX();
    point1.y = mPoint[1]->GetY();

    iBLContext.setStrokeStyle( BLRgba32(0xFFFF0000) );
    path.moveTo( point0.x, point0.y );
    path.lineTo( point1.x, point1.y );
    iBLContext.strokePath( path );
}
