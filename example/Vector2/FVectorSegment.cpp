#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

#include "FVectorSegment.h"

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
