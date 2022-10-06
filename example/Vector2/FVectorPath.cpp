#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPath::~FVectorPath()
{
}

FVectorPath::FVectorPath()
    : FVectorObject()
{

}

std::list<FVectorPoint*>&
FVectorPath::GetSelectedPointList()
{
    return mSelectedPointList;
}

FVectorSegment*
FVectorPath::AppendPoint( FVectorPoint* iPoint )
{
    mPointList.push_back( iPoint );

    return NULL;
}

void
FVectorPath::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );

    iSegment->GetPoint( 0 ).AddSegment( iSegment );
    iSegment->GetPoint( 1 ).AddSegment( iSegment );
}

void
FVectorPath::Clear()
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != --mSegmentList.end(); ++it )
    {
        FVectorSegment *segment = (*it);

        segment->GetPoint(0).RemoveSegment(segment);
        segment->GetPoint(1).RemoveSegment(segment);
        /*RemoveSegment( segment );*/ // this alters the list, hence the loop and leads to a crash
    }

    mSegmentList.erase( mSegmentList.begin(), --mSegmentList.end() );
}

void
FVectorPath::RemoveSegment( FVectorSegment* iSegment )
{
    mSegmentList.remove( iSegment );

    iSegment->GetPoint(0).RemoveSegment( iSegment );
    iSegment->GetPoint(1).RemoveSegment( iSegment );
}

FVectorSegment*
FVectorPath::GetLastSegment()
{
    if( mSegmentList.empty() == true ) return NULL;

    return mSegmentList.back();
}

FVectorPoint*
FVectorPath::GetLastPoint()
{
    if( mPointList.empty() == true ) return NULL;

    return mPointList.back();
}

void
FVectorPath::DrawStructure( FBlock& iBlock, BLContext& iBLContext )
{
    iBLContext.setStrokeStyle( BLRgba32( 0xFF00FF00 ) );
    iBLContext.setStrokeWidth(1.0f);

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->DrawStructure( iBlock, iBLContext );
    }
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
