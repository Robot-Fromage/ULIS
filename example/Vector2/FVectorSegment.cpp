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

FVectorPoint&
FVectorSegment::GetPoint( int iPointNum )
{
    return *mPoint[iPointNum];
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
                             , BLContext& iBLContext
                             , FRectD &iRoi )
{

}

FVectorSegment* FVectorSegment::GetNextSegment( )
{
    std::list<FVectorSegment*>& segmentList = mPoint[1]->GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != this ) 
        {
            return segment;
        }
    }

    return nullptr;
}

FVectorSegment* FVectorSegment::GetPreviousSegment( )
{
    std::list<FVectorSegment*>& segmentList = mPoint[0]->GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != this ) 
        {
            return segment;
        }
    }

    return nullptr;
}

void
FVectorSegment::Draw( FBlock& iBlock
                    , BLContext& iBLContext
                    , FRectD &iRoi )
{

}
