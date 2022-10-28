#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegmentSub::~FVectorSegmentSub()
{
}

FVectorSegmentSub::FVectorSegmentSub()
{
}

FVectorSegmentSub::FVectorSegmentSub( FVectorPoint* iPoint0, FVectorPoint* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
{

}
