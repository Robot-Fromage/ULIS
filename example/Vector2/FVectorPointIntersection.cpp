#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointIntersection::~FVectorPointIntersection()
{
}

FVectorPointIntersection::FVectorPointIntersection()
{

}

FVectorPointIntersection::FVectorPointIntersection( double iX, double iY, double iT )
    : FVectorPoint ( iX, iY )
    , mT ( iT )
{
    
}

uint32
FVectorPointIntersection::GetType()
{
    return FVectorPoint::POINT_TYPE_INTERSECTION; // default type;
}

double
FVectorPointIntersection::GetT()
{
    return mT;
}

void
FVectorPointIntersection::AddLoop( FVectorPathLoop* iLoop )
{
    mLoopList.push_back( iLoop );
}

void
FVectorPointIntersection::DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    for( std::list<FVectorPathLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorPathLoop* loop = static_cast<FVectorPathLoop*>(*it);

        loop->DrawShape( iBlock, iBLContext, iRoi );
    }
}
