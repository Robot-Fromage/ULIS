#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathCubic::FVectorPathCubic()
    : FVectorPath()
{

}

FVectorSegmentCubic*
FVectorPathCubic::AppendPoint( FVectorPoint* iPoint )
{
    FVectorPoint* lastPoint = GetLastPoint();

    FVectorPath::AppendPoint(iPoint);

    // lastPoint is NULL if this is the first point added
    if(lastPoint)
    {
        FVectorSegmentCubic* segment = new FVectorSegmentCubic( lastPoint, iPoint );

        AddSegment(segment);

        return segment;
    }

    return NULL;
}

void
FVectorPathCubic::Pick( double iX
                      , double iY
                      , double iRadius )
{
    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {
        FVectorPoint* point = *it;

        if( ( fabs( point->GetX() - iX ) <= iRadius ) &&
            ( fabs( point->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back(point);
        }
    }

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPoint* ctrlPoint0 = segment->GetControlPoint( 0 );
        FVectorPoint* ctrlPoint1 = segment->GetControlPoint( 1 );

        if( ( fabs( ctrlPoint0->GetX() - iX ) <= iRadius ) &&
            ( fabs( ctrlPoint0->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back(ctrlPoint0);
        }

        if( ( fabs( ctrlPoint1->GetX() - iX ) <= iRadius ) &&
            ( fabs( ctrlPoint1->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back( ctrlPoint1 );
        }
    }
}

void
FVectorPathCubic::Unselect( FVectorPoint *iPoint )
{
    if ( iPoint == NULL )
    {
        mSelectedPointList.clear();
    }
    else
    {
        mSelectedPointList.remove( iPoint );
    }
}

void
FVectorPathCubic::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{
    BLPath path;



    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));
    iBLContext.setStrokeWidth(15.0f);

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
        BLPoint point0;
        BLPoint point1;
        BLPoint ctrlPoint0;
        BLPoint ctrlPoint1;

        point0.x = segment->GetPoint(0)->GetX();
        point0.y = segment->GetPoint(0)->GetY();

        ctrlPoint0.x = segment->GetControlPoint(0)->GetX();
        ctrlPoint0.y = segment->GetControlPoint(0)->GetY();

        ctrlPoint1.x = segment->GetControlPoint(1)->GetX();
        ctrlPoint1.y = segment->GetControlPoint(1)->GetY();

        point1.x = segment->GetPoint(1)->GetX();
        point1.y = segment->GetPoint(1)->GetY();

        if ( segment->GetPoint(0)->GetSegmentCount() == 1 )
        {
            path.moveTo( point0.x, point0.y );
        }

        path.cubicTo( ctrlPoint0.x
                    , ctrlPoint0.y
                    , ctrlPoint1.x
                    , ctrlPoint1.y
                    , point1.x
                    , point1.y );
    }

    iBLContext.strokePath( path );
}
