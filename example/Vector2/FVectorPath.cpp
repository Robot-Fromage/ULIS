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
FVectorPath::AppendPoint( FVectorPoint* iPoint, FVectorPoint* iPreviousPoint )
{
    mPointList.push_back( iPoint );

    if ( iPreviousPoint )
    {
        if ( iPreviousPoint->GetSegmentCount() < 2 )
        {
            AddSegment( new FVectorSegment ( *this, iPreviousPoint, iPoint ) );
        }
    }

    return NULL;
}

void
FVectorPath::AddLoop( FVectorLoop* iLoop )
{
    mLoopList.push_back( iLoop );

    iLoop->Attach();

    iLoop->SetParent ( this );

    printf("%s: Adding loop\n", __func__ );
}

void
FVectorPath::RemoveLoop( FVectorLoop* iLoop )
{
    mLoopList.remove( iLoop );

    iLoop->Detach();

    iLoop->SetParent ( nullptr );

    printf("%s: Removing loop\n", __func__ );
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
    for ( std::list<FVectorLoop*>::iterator it = mInvalidatedLoopList.begin(); it != mInvalidatedLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

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
FVectorPath::InvalidateLoop( FVectorLoop* iLoop )
{
    mInvalidatedLoopList.push_back ( iLoop );
}

void
FVectorPath::DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    for( std::list<FVectorLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

        /*if ( loop->IsFilled() == true )
        {*/
            loop->DrawShape( iBlock, iBLContext, iRoi );
        /*}*/
    }
}

FVectorObject*
FVectorPath::PickLoops( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    for( std::list<FVectorLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

        if ( loop->PickShape( iBLContext, iX, iY, iRadius ) )
        {
            return loop;
        }
    }

    return nullptr;
}

FVectorLoop*
FVectorPath::GetLoopByID( uint64 iID )
{
    for( std::list<FVectorLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

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
