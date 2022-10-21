#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPoint* iPoint0
                                        , FVectorPoint* iPoint1 )
   : FVectorSegment( iPoint0, iPoint1 )
{
    mCtrlPoint[0].Set( iPoint0->GetX(), iPoint0->GetY() );
    mCtrlPoint[1].Set( iPoint1->GetX(), iPoint1->GetY() );
}

FVectorPointCubic&
FVectorSegmentCubic::GetPoint( int iPointNum )
{
    return static_cast<FVectorPointCubic&>( FVectorSegment::GetPoint( iPointNum ) );
}

FVec2D FVectorSegmentCubic::GetVectorAtEnd( bool iNormalize )
{
    FVec2D vec = { GetPoint(1).GetX() - GetControlPoint(1).GetX(),
                   GetPoint(1).GetY() - GetControlPoint(1).GetY() };

    if( iNormalize && vec.DistanceSquared() )
    {
        vec.Normalize();
    }

    return vec;
}

FVec2D FVectorSegmentCubic::GetVectorAtStart( bool iNormalize )
{
    FVec2D vec = { GetControlPoint(0).GetX() - GetPoint(0).GetX(),
                   GetControlPoint(0).GetY() - GetPoint(0).GetY() };

    if( iNormalize && vec.DistanceSquared() )
    {
        vec.Normalize();
    }

    return vec;
}

FVec2D FVectorSegmentCubic::GetNextVector( bool iNormalize )
{
    std::list<FVectorSegment*>& segmentList = mPoint[1]->GetSegmentList();
     static FVec2D zero = { 0.0f, 0.0f };

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);

        if ( segment != this ) 
        {
            return segment->GetVectorAtStart ( iNormalize );
        }
    }

    return zero;
}

FVec2D FVectorSegmentCubic::GetPreviousVector( bool iNormalize )
{
    std::list<FVectorSegment*>& segmentList = mPoint[0]->GetSegmentList();
     static FVec2D zero = { 0.0f, 0.0f };

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);

        if ( segment != this ) 
        {
            return segment->GetVectorAtEnd ( iNormalize );
        }
    }

    return zero;
}

FVectorHandleSegment&
FVectorSegmentCubic::GetControlPoint( int iCtrlPointNum )
{
    return mCtrlPoint[iCtrlPointNum];
}

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPoint* iPoint0
                                        , double iCtrlPoint0x
                                        , double iCtrlPoint0y
                                        , double iCtrlPoint1x
                                        , double iCtrlPoint1y
                                        , FVectorPoint* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
{
    mCtrlPoint[0].Set( iCtrlPoint0x, iCtrlPoint0y );
    mCtrlPoint[1].Set( iCtrlPoint1x, iCtrlPoint1y );
}

void
FVectorSegmentCubic::DrawStructure( FBlock& iBlock
                                  , BLContext& iBLContext )
{
    BLPath ctrlPath0;
    BLPath ctrlPath1;

    BLPoint point0;
    BLPoint point1;
    BLPoint ctrlPoint0;
    BLPoint ctrlPoint1;

    point0.x = mPoint[0]->GetX();
    point0.y = mPoint[0]->GetY();

    ctrlPoint0.x = mCtrlPoint[0].GetX();
    ctrlPoint0.y = mCtrlPoint[0].GetY();

    ctrlPoint1.x = mCtrlPoint[1].GetX();
    ctrlPoint1.y = mCtrlPoint[1].GetY();

    point1.x = mPoint[1]->GetX();
    point1.y = mPoint[1]->GetY();

    ctrlPath0.moveTo( point0.x, point0.y );
    ctrlPath0.lineTo( ctrlPoint0.x,ctrlPoint0.y );

    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );
    iBLContext.setStrokeWidth( 1.0f );

    iBLContext.strokePath( ctrlPath0 );

    ctrlPath1.moveTo( point1.x, point1.y );
    ctrlPath1.lineTo( ctrlPoint1.x,ctrlPoint1.y );

    iBLContext.strokePath( ctrlPath1 );

    iBLContext.setFillStyle( BLRgba32( 0xFFFF0000 ) );
    iBLContext.fillRect( ctrlPoint0.x - 2, ctrlPoint0.y - 2, 4, 4 );
    iBLContext.fillRect( ctrlPoint1.x - 2, ctrlPoint1.y - 2, 4, 4 );
}

void
FVectorSegmentCubic::Draw( FBlock& iBlock
                         , BLContext& iBLContext )
{

}

