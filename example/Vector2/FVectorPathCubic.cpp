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

bool
FVectorPathCubic::PickShape( double iX
                           , double iY
                           , double iRadius )
{

    return false;
}

void
FVectorPathCubic::PickPoint( double iX
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
FVectorPathCubic::DrawStructure( FBlock& iBlock, BLContext& iBLContext )
{
    BLPath path;

    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
        BLPoint point0;
        BLPoint point1;
        BLPoint ctrlPoint0;
        BLPoint ctrlPoint1;

        point0.x = segment->GetPoint( 0 )->GetX();
        point0.y = segment->GetPoint( 0 )->GetY();

        ctrlPoint0.x = segment->GetControlPoint( 0 )->GetX();
        ctrlPoint0.y = segment->GetControlPoint( 0 )->GetY();

        ctrlPoint1.x = segment->GetControlPoint( 1 )->GetX();
        ctrlPoint1.y = segment->GetControlPoint( 1 )->GetY();

        point1.x = segment->GetPoint( 1 )->GetX();
        point1.y = segment->GetPoint( 1 )->GetY();

        if ( segment->GetPoint( 0 )->GetSegmentCount() == 1 )
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

    iBLContext.setStrokeStyle( BLRgba32( 0xFF00FF00 ) );
    iBLContext.setStrokeWidth( 1.0f );
    iBLContext.strokePath( path );

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);

        segment->DrawStructure( iBlock, iBLContext );
    }

    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {
        FVectorPoint *point = static_cast<FVectorPoint*>(*it);

        iBLContext.setFillStyle( BLRgba32( 0xFFFF00FF ) );
        iBLContext.fillRect( point->GetX() - 3, point->GetY() - 3, 6, 6 );
    }
}

void
FVectorPathCubic::DrawSegment( BLPath& iPath
                             , FVectorSegmentCubic& iSegment
                             , FVec2D* iDrift0
                             , FVec2D* iDrift1 )
{
    BLPoint point0;
    BLPoint point1;
    BLPoint ctrlPoint0;
    BLPoint ctrlPoint1;

    point0.x = iSegment.GetPoint(0)->GetX();
    point0.y = iSegment.GetPoint(0)->GetY();

    ctrlPoint0.x = iSegment.GetControlPoint(0)->GetX();
    ctrlPoint0.y = iSegment.GetControlPoint(0)->GetY();

    ctrlPoint1.x = iSegment.GetControlPoint(1)->GetX();
    ctrlPoint1.y = iSegment.GetControlPoint(1)->GetY();

    point1.x = iSegment.GetPoint(1)->GetX();
    point1.y = iSegment.GetPoint(1)->GetY();

    if( iDrift0 )
    {
        point0.x += iDrift0->x;
        point0.y += iDrift0->y;

        ctrlPoint0.x += iDrift0->x;
        ctrlPoint0.y += iDrift0->y;
    }

    if( iDrift1 )
    {
        point1.x += iDrift1->x;
        point1.y += iDrift1->y;

        ctrlPoint1.x += iDrift1->x;
        ctrlPoint1.y += iDrift1->y;
    }

    if ( iSegment.GetPoint(0)->GetSegmentCount() == 1 )
    {
        /*iPath.moveTo( point0.x, point0.y );*/
        iPath.lineTo( point0.x, point0.y );
    }

    iPath.cubicTo( ctrlPoint0.x
                 , ctrlPoint0.y
                 , ctrlPoint1.x
                 , ctrlPoint1.y
                 , point1.x
                 , point1.y );
}

void
FVectorPathCubic::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{
/*
    BLPath path;

    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);

    iBLContext.setStrokeStyle( BLRgba32( mStrokeColor ) );
    iBLContext.setStrokeWidth( mStrokeWidth );

    path.clear();

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);

        DrawSegment( path, *segment, nullptr, nullptr );
    }

    iBLContext.strokePath( path );

    DrawStructure( iBlock, iBLContext );
*/
DrawShapeVariable( iBlock, iBLContext );
}

void
FVectorPathCubic::DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext )
{
    BLPath path0;
    BLPath path1;
    BLPoint perpendicularPoint0Right = { 0.0f, 0.0f };
    BLPoint perpendicularPoint0Left = { 0.0f, 0.0f };
    BLPoint perpendicularPoint1Right = { 0.0f, 0.0f };
    BLPoint perpendicularPoint1Left = { 0.0f, 0.0f };

    double mStartRadius = 10.0f;
    double mEndRadius = 0.0f;

    iBLContext.setCompOp( BL_COMP_OP_SRC_COPY );

    iBLContext.setStrokeStyle( BLRgba32( mStrokeColor ) );
    iBLContext.setStrokeWidth( mStrokeWidth );

    if( mSegmentList.size() )
    {
        double stepRadius = ( mEndRadius - mStartRadius ) / mSegmentList.size();
        double segmentStartRadius = mStartRadius;
        double segmentEndRadius = segmentStartRadius + stepRadius;

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            BLPoint point0;
            BLPoint point1;
            BLPoint ctrlPoint0;
            BLPoint ctrlPoint1;
            BLPoint samplePoint0;
            BLPoint samplePoint1;
            FVec2D  parallelVec0;
            FVec2D  parallelVec1;

            point0.x = segment->GetPoint(0)->GetX();
            point0.y = segment->GetPoint(0)->GetY();

            ctrlPoint0.x = segment->GetControlPoint(0)->GetX();
            ctrlPoint0.y = segment->GetControlPoint(0)->GetY();

            ctrlPoint1.x = segment->GetControlPoint(1)->GetX();
            ctrlPoint1.y = segment->GetControlPoint(1)->GetY();

            point1.x = segment->GetPoint(1)->GetX();
            point1.y = segment->GetPoint(1)->GetY();

            samplePoint0 = CubicBezierPointAtParameter<BLPoint>( point0
                                                               , ctrlPoint0
                                                               , ctrlPoint1
                                                               , point1
                                                               , 0.01f );

            samplePoint1   = CubicBezierPointAtParameter<BLPoint>( point0
                                                                 , ctrlPoint0
                                                                 , ctrlPoint1
                                                                 , point1
                                                                 , 0.99f );

            parallelVec0.x =   ( samplePoint0.y - point0.y );
            parallelVec0.y = - ( samplePoint0.x - point0.x );

            parallelVec1.x   =   ( point1.y - samplePoint1.y );
            parallelVec1.y   = - ( point1.x - samplePoint1.x );

            if ( parallelVec0.Distance() && parallelVec1.Distance() )
            {
                FVec2D perpendicularVec0Right;
                FVec2D perpendicularVec0Left;
                FVec2D perpendicularVec1Right;
                FVec2D perpendicularVec1Left;

                parallelVec0.Normalize();
                parallelVec1.Normalize();

                perpendicularVec0Right =   ( parallelVec0 * segmentStartRadius );
                perpendicularVec0Left  = - ( perpendicularVec0Right );

                perpendicularVec1Right =   ( parallelVec1 * segmentEndRadius   );
                perpendicularVec1Left  = - ( perpendicularVec1Right   );

                // remember this point to close the path at its inception
                if( it == mSegmentList.begin() )
                {
                    perpendicularPoint0Right.x = point0.x + perpendicularVec0Right.x;
                    perpendicularPoint0Right.y = point0.y + perpendicularVec0Right.y;

                    perpendicularPoint0Left.x  = point0.x + perpendicularVec0Left.x;
                    perpendicularPoint0Left.y  = point0.y + perpendicularVec0Left.y;

                    path0.moveTo( perpendicularPoint0Left  );
                    path0.lineTo( perpendicularPoint0Right );
                }

                DrawSegment( path0, *segment, &perpendicularVec0Right, &perpendicularVec1Right );
                DrawSegment( path1, *segment, &perpendicularVec0Left , &perpendicularVec1Left  );

                // remember this point to close the path at its inception
                if( std::next( it, 1 ) ==  mSegmentList.end() )
                {
                    perpendicularPoint1Left.x  = point1.x + perpendicularVec1Left.x;
                    perpendicularPoint1Left.y  = point1.y + perpendicularVec1Left.y;

                    path0.lineTo( perpendicularPoint1Left );
                }
            }

            segmentStartRadius = segmentEndRadius;
            segmentEndRadius  += stepRadius;
        }
    }

    /*iBLContext.strokePath( path0 );
    iBLContext.strokePath( path1 );*/

    /*path0.addPath( path1 );*/
    path0.addReversedPath( path1, BL_PATH_REVERSE_MODE_COMPLETE );
    path0.lineTo( perpendicularPoint1Left );

    iBLContext.setStrokeWidth(1.0f);
    /*iBLContext.strokePath(path0);*/

    iBLContext.setFillStyle( BLRgba32( mFillColor ) );
    iBLContext.fillPath( path0 );

    DrawStructure( iBlock, iBLContext );
}