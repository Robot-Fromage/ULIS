#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPoint* iPoint0
                                        , FVectorPoint* iPoint1 )
   : FVectorSegment( iPoint0, iPoint1 )
{
    mCtrlPoint[0].Set( iPoint0->GetX(), iPoint0->GetY() );
    mCtrlPoint[1].Set( iPoint1->GetX(), iPoint1->GetY() );
}

FVectorPoint*
FVectorSegmentCubic::GetControlPoint( int iCtrlPointNum )
{
    return &mCtrlPoint[iCtrlPointNum];
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
FVectorSegmentCubic::Draw( FBlock& iBlock
                         , BLContext& iBLContext )
{
    BLPath path;
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
/*
    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );
    ctrlPath0.moveTo( point0.x, point0.y );
    ctrlPath0.lineTo( ctrlPoint0.x, ctrlPoint0.y );
    iBLContext.strokePath( ctrlPath0 );

    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );
    ctrlPath1.moveTo( point1.x, point1.y );
    ctrlPath1.lineTo( ctrlPoint1.x, ctrlPoint1.y );
    iBLContext.strokePath( ctrlPath1 );
*/
    iBLContext.setStrokeStyle( BLRgba32( 0xFF000000 ) );
    path.moveTo( point0.x, point0.y );
    path.cubicTo( ctrlPoint0.x
                , ctrlPoint0.y
                , ctrlPoint1.x
                , ctrlPoint1.y
                , point1.x
                , point1.y );
    iBLContext.strokePath( path );
}

