#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPath::~FVectorPath()
{
}

FVectorPath::FVectorPath()
    : FVectorObject()
{

}

FVectorPath::FVectorPath( std::string iName )
    : FVectorObject( iName )
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
FVectorPath::AddLoop( FVectorPathLoop* iLoop )
{
    mLoopList.push_back( iLoop );

    iLoop->mParent = this;
}

void
FVectorPath::UpdateShape()
{
    // update segments
    for ( std::list<FVectorSegment*>::iterator it = mInvalidatedSegmentList.begin(); it != mInvalidatedSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        segment->Update();
    }

    mInvalidatedSegmentList.clear();

    // then update Loops
    for ( std::list<FVectorPathLoop*>::iterator it = mInvalidatedLoopList.begin(); it != mInvalidatedLoopList.end(); ++it )
    {
        FVectorPathLoop* loop = static_cast<FVectorPathLoop*>(*it);

        loop->UpdateShape();
    }

    mInvalidatedLoopList.clear();
}

void
FVectorPath::InvalidateSegment( FVectorSegment* iSegment )
{
    mInvalidatedSegmentList.push_back( iSegment );
}

void
FVectorPath::InvalidateLoop( FVectorPathLoop* iLoop )
{
    mInvalidatedLoopList.push_back ( iLoop );
}

void
FVectorPath::DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    for( std::list<FVectorPathLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorPathLoop* loop = static_cast<FVectorPathLoop*>(*it);

        /*if ( loop->IsFilled() == true )
        {*/
            loop->DrawShape( iBlock, iBLContext, iRoi );
        /*}*/
    }
}

FVectorObject*
FVectorPath::PickLoops( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    for( std::list<FVectorPathLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorPathLoop* loop = static_cast<FVectorPathLoop*>(*it);

        if ( loop->PickShape( iBLContext, iX, iY, iRadius ) )
        {
            return loop;
        }
    }

    return nullptr;
}

FVectorPathLoop*
FVectorPath::GetPathLoopByID( uint64 iID )
{
    for( std::list<FVectorPathLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorPathLoop* loop = static_cast<FVectorPathLoop*>(*it);

        if ( iID == loop->GetID() )
        {
            return loop;
        }
    }

    return nullptr;
}

void
FVectorPath::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );

    iSegment->GetPoint(0)->AddSegment( iSegment );
    iSegment->GetPoint(1)->AddSegment( iSegment );
}

void
FVectorPath::Clear()
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != --mSegmentList.end(); ++it )
    {
        FVectorSegment *segment = (*it);

        segment->GetPoint(0)->RemoveSegment(segment);
        segment->GetPoint(1)->RemoveSegment(segment);
        /*RemoveSegment( segment );*/ // this alters the list, hence the loop and leads to a crash
    }

    mSegmentList.erase( mSegmentList.begin(), --mSegmentList.end() );
}

void
FVectorPath::RemoveSegment( FVectorSegment* iSegment )
{
    mSegmentList.remove( iSegment );

    iSegment->GetPoint(0)->RemoveSegment( iSegment );
    iSegment->GetPoint(1)->RemoveSegment( iSegment );
}

std::list<FVectorSegment*>&
FVectorPath::GetSegmentList()
{
    return mSegmentList;
}

FVectorSegment*
FVectorPath::GetLastSegment()
{
    if( mSegmentList.size() == 0 ) return nullptr;

    return mSegmentList.back();
}

FVectorPoint*
FVectorPath::GetLastPoint()
{
    if( mPointList.size() == 0 ) return nullptr;

    return mPointList.back();
}

bool
FVectorPath::IsLoop()
{
    if ( mPointList.size() )
    {
        if ( mPointList.size() == mSegmentList.size() )
        {
            return true;
        }
    }

    return false;
}

FVectorPoint*
FVectorPath::GetFirstPoint()
{
    if( mPointList.size() == 0 ) return nullptr;

    return mPointList.front();
}

void
FVectorPath::DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    iBLContext.setStrokeStyle( BLRgba32( 0xFF00FF00 ) );
    iBLContext.setStrokeWidth(1.0f);

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->DrawStructure( iBlock, iBLContext, iRoi );
    }
}

void
FVectorPath::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{


    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->Draw( iBlock, iBLContext, iRoi );
    }

    DrawLoops( iBlock, iBLContext, iRoi );
}
