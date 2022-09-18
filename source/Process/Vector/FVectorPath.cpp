#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

FVectorPath::~FVectorPath()
{
}

FVectorPath::FVectorPath()
{
    mLastPoint = NULL;
}

std::list<FVectorPoint*> 
FVectorPath::GetSelectedPointList()
{
    return mSelectedPointList;
}

FVectorSegment*
FVectorPath::AppendPoint( FVectorPoint* iPoint )
{
    mPointList.push_back( iPoint );
    mLastPoint = iPoint;

    return NULL;
}

void
FVectorPath::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );
}

FVectorPoint*
FVectorPath::GetLastPoint()
{
    return mLastPoint;
}

void
FVectorPath::SetLastPoint(FVectorPoint* iLastPoint)
{
    mLastPoint = iLastPoint;
}

void
FVectorPath::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{


    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->Draw( iBlock, iBLContext );
    }
}
