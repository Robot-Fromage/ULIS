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

FVectorObject*
FVectorPath::CopyShape()
{
    return nullptr;
}

void
FVectorPath::UpdateBBox()
{
    double x1 = DBL_MAX, y1 = DBL_MAX, x2 = -DBL_MAX, y2 = -DBL_MAX;

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);
        FRectD& coords = segment->GetBoundingBox();
        double rx1 = coords.x, ry1 = coords.y, rx2 = coords.x + coords.w, ry2 = coords.y + coords.h;

        if ( rx1 < x1 ) x1 = rx1;
        if ( ry1 < y1 ) y1 = ry1;
        if ( rx2 > x2 ) x2 = rx2;
        if ( ry2 > y2 ) y2 = ry2;
    }

    mBBox = TRectangle<double>::FromMinMax( x1, y1, x2, y2 );
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

        loop->Update();
    }

    mInvalidatedLoopList.clear();

    UpdateBBox();
}

void
FVectorPath::InvalidateSegment( FVectorSegment* iSegment )
{
    mInvalidatedSegmentList.push_back( iSegment );

    Invalidate();
}

void
FVectorPath::InvalidateLoop( FVectorLoop* iLoop )
{
    mInvalidatedLoopList.push_back ( iLoop );

    Invalidate();
}

void
FVectorPath::DrawLoops( FRectD &iRoi, uint64 iFlags )
{
    for( std::list<FVectorLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

        /*if ( loop->IsFilled() == true )
        {*/
            loop->DrawShape( iRoi, iFlags );
        /*}*/
    }
}

FVectorObject*
FVectorPath::PickLoops( double iX, double iY, double iRadius )
{
    for( std::list<FVectorLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

        if ( loop->PickShape( iX, iY, iRadius ) )
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
FVectorPath::AddPoint( FVectorPoint* iPoint )
{
    mPointList.push_back( iPoint );
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

    mSegmentList.clear();
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

FVectorSegment*
FVectorPath::GetFirstSegment()
{
    if( mSegmentList.size() == 0 ) return nullptr;

    return mSegmentList.front();
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
FVectorPath::DrawStructure( FRectD& iRoi )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    blctx.setStrokeStyle( BLRgba32( 0xFF00FF00 ) );
    blctx.setStrokeWidth(1.0f);

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->DrawStructure( iRoi );
    }
}

void
FVectorPath::DrawShape( FRectD& iRoi, uint64 iFlags )
{
    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->Draw( iRoi );
    }

    DrawLoops( iRoi, iFlags );
}