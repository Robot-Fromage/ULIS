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
}

FVectorPoint*
FVectorSegment::GetPoint( int iPointNum )
{
    return mPoint[iPointNum];
}

double
FVectorSegment::GetStraightDistance()
{
    FVec2D vec = { mPoint[1]->GetX() - mPoint[0]->GetX(),
                   mPoint[1]->GetY() - mPoint[0]->GetY() };

    return vec.Distance();
}

void
FVectorSegment::DrawStructure( FBlock& iBlock
                             , BLContext& iBLContext )
{

}

void
FVectorSegment::Draw( FBlock& iBlock
                    , BLContext& iBLContext )
{

}
