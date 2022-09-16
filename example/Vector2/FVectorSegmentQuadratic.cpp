#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

#include "FVectorSegmentQuadratic.h"

FVectorSegmentQuadratic::~FVectorSegmentQuadratic()
{
}

FVectorSegmentQuadratic::FVectorSegmentQuadratic( FVectorPoint* iPoint0
                                                , double iCtrlx
                                                , double iCtrly
                                                , FVectorPoint* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
    ,mCtrlPoint( iCtrlx, iCtrly )
{

}

FVectorSegmentQuadratic::FVectorSegmentQuadratic( FVectorPoint* iPoint0
                                                ,FVectorPoint* iPoint1)
    : FVectorSegment(iPoint0,iPoint1)
{

}

void
FVectorSegmentQuadratic::Draw( FBlock& iBlock
                             , BLContext& iBLContext )
{
    BLPath path;
    BLPath ctrlPath0;
    BLPath ctrlPath1;

    BLPoint point0;
    BLPoint point1;
    BLPoint ctrlPoint;

    point0.x = mPoint[0]->GetX();
    point0.y = mPoint[0]->GetY();

    ctrlPoint.x = mCtrlPoint.GetX();
    ctrlPoint.y = mCtrlPoint.GetY();

    point1.x = mPoint[1]->GetX();
    point1.y = mPoint[1]->GetY();

    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF0000 ));
    ctrlPath0.moveTo( point0.x, point0.y );
    ctrlPath0.lineTo( ctrlPoint.x, ctrlPoint.y );
    iBLContext.strokePath( ctrlPath0 );

    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );
    ctrlPath1.moveTo( point1.x, point1.y );
    ctrlPath1.lineTo( ctrlPoint.x, ctrlPoint.y );
    iBLContext.strokePath( ctrlPath1 );

    iBLContext.setStrokeStyle( BLRgba32( 0xFF000000 ) );
    path.moveTo( point0.x,point0.y );
    path.quadTo(  ctrlPoint.x
                , ctrlPoint.y
                , point1.x
                , point1.y );
    iBLContext.strokePath( path );
}
