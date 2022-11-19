#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathBuilder::~FVectorPathBuilder()
{

}

FVectorPathBuilder::FVectorPathBuilder( FVectorPathCubic* iCubicPath )
    : mCubicPath ( iCubicPath )
    , mCumulAngle ( 0.0f )
    , mCumulAngleLimit ( 1.5708f ) /* 90 degrees */
    , mLastCubicAngleLimit ( 2.35619f ) /* 135 degrees */
{
    mStrokeColor = 0xFF808080;
}

FVectorSegment*
FVectorPathBuilder::GetLastSampleSegment()
{
    if( mSampleSegmentList.size() == 0 ) return nullptr;

    return mSampleSegmentList.back();
}

FVectorPoint*
FVectorPathBuilder::GetLastSamplePoint()
{
    if( mSamplePointList.size() == 0 ) return nullptr;

    return mSamplePointList.back();
}

FVectorObject*
FVectorPathBuilder::CopyShape()
{
    return nullptr;
}

void
FVectorPathBuilder::Round( FVectorSegmentCubic& cubicSegment
                         , FVec2D iFirstVector
                         , FVec2D iLastVector )
{
    FVectorSegmentCubic* previousCubicSegment = static_cast<FVectorSegmentCubic*>(cubicSegment.GetPreviousSegment());
    double segmentLength = cubicSegment.GetStraightDistance();

    if ( previousCubicSegment )
    {
        FVec2D previousSegmentVector = previousCubicSegment->GetVectorAtEnd( true );
        FVec2D cubicSegmentVector = cubicSegment.GetVector( true );
        double dot = cubicSegmentVector.DotProduct( - previousSegmentVector );
        double angle = acos( ULIS::FMath::Clamp<double>( dot, -1.0f, 1.0f ) );

        if ( angle > mLastCubicAngleLimit )
        {
            FVec2D averageVector = previousSegmentVector + cubicSegmentVector;
            double length = previousCubicSegment->GetStraightDistance();

            if ( averageVector.DistanceSquared() )
            {
                averageVector.Normalize();

                previousCubicSegment->GetControlPoint(1).SetX( previousCubicSegment->GetPoint(1)->GetX() - ( averageVector.x * length * 0.35f ) );
                previousCubicSegment->GetControlPoint(1).SetY( previousCubicSegment->GetPoint(1)->GetY() - ( averageVector.y * length * 0.35f ) );

                iFirstVector.x = averageVector.x;
                iFirstVector.y = averageVector.y;

                previousCubicSegment->Update();
            }
        }
    }

    cubicSegment.GetControlPoint(0).Set( cubicSegment.GetPoint(0)->GetX() + ( iFirstVector.x * segmentLength * 0.35f )
                                       , cubicSegment.GetPoint(0)->GetY() + ( iFirstVector.y * segmentLength * 0.35f ) );

    cubicSegment.GetControlPoint(1).Set( cubicSegment.GetPoint(1)->GetX() + ( iLastVector.x  * segmentLength * 0.35f )
                                       , cubicSegment.GetPoint(1)->GetY() + ( iLastVector.y  * segmentLength * 0.35f ) );

    cubicSegment.Update();
}

FVectorSegmentCubic*
FVectorPathBuilder::Sample( FVectorPoint* iSamplePoint, double iRadius, bool iEnforce )
{
    FVectorPoint* lastSamplePoint = GetLastSamplePoint();
    FVectorSegmentCubic* cubicSegment = nullptr;

    mSamplePointList.push_back( iSamplePoint );

    if ( lastSamplePoint )
    {
        FVectorSegment* lastSampleSegment = GetLastSampleSegment();
        FVectorSegment* sampleSegment = new FVectorSegment ( *this, lastSamplePoint, iSamplePoint );

        mSampleSegmentList.push_back( sampleSegment );

        if ( lastSampleSegment )
        {
            FVec2D lastSampleSegmentVector = lastSampleSegment->GetVector( true );
            FVec2D     sampleSegmentVector =     sampleSegment->GetVector( true );
            double dot = lastSampleSegmentVector.DotProduct( sampleSegmentVector );
            double angle = acos( ULIS::FMath::Clamp<double>( dot, -1.0f, 1.0f ) );

            mCumulAngle += angle;

            if (/* ( abs(angle) > 0.785398f )*/ /* 45 degrees */ /*||*/ ( mCumulAngle >= mCumulAngleLimit ) || iEnforce )
            {
                FVectorPointCubic* cubicPoint = new FVectorPointCubic( lastSamplePoint->GetX()
                                                                     , lastSamplePoint->GetY()
                                                                     , iRadius );
                FVec2D p0Vector = mSampleSegmentList.front()->GetVector( true );
                FVec2D p1Vector = - lastSampleSegment->GetVector( true );

                cubicSegment = mCubicPath->AppendPoint( cubicPoint, true, true );

                Round ( *cubicSegment
                        , p0Vector
                        , p1Vector );

                mCumulAngle = 0.0f;
            }
        }
    }

    return cubicSegment;
}

FVectorSegment*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY
                               , double iRadius
                               , bool   iEnforce )
{
    FVectorPoint* point = new FVectorPoint( iX, iY );
    FVectorSegment* segment = nullptr;
    FVectorPoint* lastPoint = GetLastPoint();

    segment = FVectorPath::AppendPoint( point, lastPoint );

    if ( lastPoint == nullptr )
    {
        FVectorPointCubic* cubicPoint = new FVectorPointCubic( point->GetX()
                                                             , point->GetY()
                                                             , iRadius );

        mCubicPath->AppendPoint( cubicPoint, false, false );

        Sample ( point, iRadius, iEnforce );
    }
    else
    {
        FVectorPoint* lastSamplePoint = GetLastSamplePoint();

        FVec2D dif = { iX - lastSamplePoint->GetX()
                     , iY - lastSamplePoint->GetY() };
        double length = dif.Distance();

        if ( length >= 20.0f || iEnforce == true )
        {
            FVectorPoint* samplePoint = new FVectorPoint ( iX, iY );
            FVectorSegmentCubic* cubicSegment = Sample ( samplePoint, iRadius, iEnforce );

            if ( cubicSegment )
            {
                mSampleSegmentList.clear();
                mSamplePointList.clear();
                mSegmentList.clear();
                mPointList.clear();

                // TODO, Free memory except for lastSamplePoint

                // restart
                FVectorPath::AppendPoint( lastSamplePoint, nullptr );
                Sample ( lastSamplePoint, iRadius, false );
            }
        }
    }

    return segment;
}

#ifdef OLDVER
FVectorSegment*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY
                               , double iRadius
                               , bool   iEnforce )
{
    FVectorPoint* lastPoint = GetLastPoint();

    if ( mCubicPath )
    {
    // we already have a loop. don't go further
        if ( mCubicPath->IsLoop() == true )
        {
            return nullptr;
        }

        // lastPoint is NULL if this is the first point added
        if( lastPoint )
        {
            // remember the last segment before it gets changed by the call to the base AddSegment();
            FVectorSegment* lastSegment = GetLastSegment() /*mLastSegment*/;
            double difX = ( iX - lastPoint->GetX() );
            double difY = ( iY - lastPoint->GetY() );
            double distanceSq = ( difX * difX ) + ( difY * difY );

            // do not pick to many input points
            if ( ( distanceSq > 144 ) || iEnforce == true )
            {
                FVec2D segVector( iX - lastPoint->GetX()
                                , iY - lastPoint->GetY() );
                double segVectorDistance = segVector.Distance();

                if( segVectorDistance )
                {
                    FVectorPoint* iPoint = new FVectorPoint( iX, iY );
                    FVectorSegment* segment = new FVectorSegment( *this, lastPoint, iPoint );

                    FVectorPath::AddSegment( segment );
                    FVectorPath::AppendPoint( iPoint );

                    segVector.Normalize();

                    // if that's the first segment, prepare some cubic controller vector values
                    if( lastSegment == nullptr )
                    {
                        mLastSmoothedSegmentVector.x = segVector.x;
                        mLastSmoothedSegmentVector.y = segVector.y;
                        mLastSmoothedSegmentVectorDistance = segVectorDistance;
                    }

                    if( lastSegment )
                    {
                        double lastSegmentTotalX = ( lastSegment->GetPoint(0)->GetX() + lastSegment->GetPoint(1)->GetX() );
                        double lastSegmentTotalY = ( lastSegment->GetPoint(0)->GetY() + lastSegment->GetPoint(1)->GetY() );
                        double     segmentTotalX = (     segment->GetPoint(0)->GetX() +     segment->GetPoint(1)->GetX() );
                        double     segmentTotalY = (     segment->GetPoint(0)->GetY() +     segment->GetPoint(1)->GetY() );

                        FVec2D lastSegVector( lastSegment->GetPoint(1)->GetX() - lastSegment->GetPoint(0)->GetX()
                                            , lastSegment->GetPoint(1)->GetY() - lastSegment->GetPoint(0)->GetY() );

                        double lastSegVectorDistance = lastSegVector.Distance();

                        // actually this is unneeded because the segment's length was checked at the previous call
                        if( lastSegVectorDistance )
                        {
                            double angle = 0.0f;
                            double angleLimit = 90.0f;

                            static double cumulAngle = 0.0f;

                            lastSegVector.Normalize();

                            angle = acos(ULIS::FMath::Clamp<double>(segVector.DotProduct( lastSegVector ), -1.0f, 1.0f)) / 3.14159 * 180;
                            cumulAngle += angle;

                            double ratio = lastSegVector.Distance() / segVector.Distance();

                            if( ( cumulAngle > angleLimit ) || iEnforce == true )
                            {
                                FVectorPointCubic* lastSmoothedPoint = static_cast<FVectorPointCubic*>( mCubicPath->GetLastPoint() );
                                FVectorSegmentCubic* lastCubicSegment =  static_cast<FVectorSegmentCubic*>( mCubicPath->GetLastSegment() );
                                FVectorPointCubic* firstPoint = static_cast<FVectorPointCubic*>(  mCubicPath->GetFirstPoint() );
                                FVec2D lastPointToFirstVector = { firstPoint->GetX() - lastPoint->GetX()
                                                                , firstPoint->GetY() - lastPoint->GetY() };
                                FVectorPointCubic* cubicPoint;

                                // go for a loop
                                if ( lastPointToFirstVector.DistanceSquared() < 64.0f )
                                {
                                    cubicPoint = firstPoint;
printf("looping\n");
                                }
                                // dont loop
                                else
                                {
                                    cubicPoint = new FVectorPointCubic( lastPoint->GetX()
                                                                      , lastPoint->GetY()
                                                                      , iRadius );
                                }

                                FVectorSegmentCubic* cubicSegment = mCubicPath->AppendPoint( cubicPoint, true, false );

                                double cubicSegmentStraightDistance = cubicSegment->GetStraightDistance();
                                // Note: lastSegVector is now normalized

                                FVec2D ctrlPt0 = { lastSmoothedPoint->GetX() + mLastSmoothedSegmentVector.x * cubicSegmentStraightDistance * 0.3f
                                                 , lastSmoothedPoint->GetY() + mLastSmoothedSegmentVector.y * cubicSegmentStraightDistance * 0.3f };
                                FVec2D ctrlPt1 = { lastPoint->GetX() - lastSegVector.x * cubicSegmentStraightDistance * 0.3f
                                                 , lastPoint->GetY() - lastSegVector.y * cubicSegmentStraightDistance * 0.3f };

                                cubicSegment->GetControlPoint( 0 ).SetX( ctrlPt0.x );
                                cubicSegment->GetControlPoint( 0 ).SetY( ctrlPt0.y );
                                cubicSegment->GetControlPoint( 1 ).SetX( ctrlPt1.x );
                                cubicSegment->GetControlPoint( 1 ).SetY( ctrlPt1.y );

                                if( cubicPoint->GetSegmentCount() == 2 && lastCubicSegment )
                                {
                                    double mRoundedAngle = acos(ULIS::FMath::Clamp<double>(mLastSmoothedSegmentVector.DotProduct(lastSegVector),-1.0f,1.0f)) / 3.14159 * 180;

                                    if( mLastSmoothedAngle < 45.0f )
                                    {
                                        double lastCubicSegmentStraightDistance = lastCubicSegment->GetStraightDistance();

                                        FVec2D vec = { lastCubicSegment->GetPoint(1)->GetX() - lastCubicSegment->GetControlPoint(1).GetX(),
                                                       lastCubicSegment->GetPoint(1)->GetY() - lastCubicSegment->GetControlPoint(1).GetY() };

                                        if ( vec.DistanceSquared())
                                            vec.Normalize();

                                        // uncomment to round based on average vectors
                                        vec = ( vec + mLastSmoothedSegmentVector ) * 0.5f;

                                        FVec2D ctrlPtcurr = { lastSmoothedPoint->GetX() + vec.x * cubicSegmentStraightDistance     * 0.3f
                                                            , lastSmoothedPoint->GetY() + vec.y * cubicSegmentStraightDistance     * 0.3f };
                                        FVec2D ctrlPtprev = { lastSmoothedPoint->GetX() - vec.x * lastCubicSegmentStraightDistance * 0.3f
                                                            , lastSmoothedPoint->GetY() - vec.y * lastCubicSegmentStraightDistance * 0.3f };

                                        lastCubicSegment->GetControlPoint( 1 ).Set( ctrlPtprev.x, ctrlPtprev.y );
                                            cubicSegment->GetControlPoint( 0 ).Set( ctrlPtcurr.x, ctrlPtcurr.y );
                                    }
                                    /*else
                                    {
                                        cubicSegment->GetControlPoint(0)->SetX(lastSmoothedPoint->GetX());
                                        cubicSegment->GetControlPoint(0)->SetY(lastSmoothedPoint->GetY());
                                    }*/
                                }

                                // this must be done after setting the control points, as segment 
                                // building needs control points to be at different position from 
                                // the points at the tips of the segments.
                                cubicSegment->GetPoint(0)->InvalidateSegments();
                                mCubicPath->UpdateShape();

                                cubicSegment->IntersectPath( *mCubicPath );

                                /*mSmoothedPointList.push_back( lastPoint );*/

                                mLastSmoothedSegmentVector.x = segVector.x;
                                mLastSmoothedSegmentVector.y = segVector.y;
                                mLastSmoothedSegmentVectorDistance = segVectorDistance;

                                mLastSmoothedAngle = angle;

                                cumulAngle = 0;

                                Clear();
                            }
                        }
                    }

                    return segment;
                }
            }
        }
        else
        {
            FVectorPath::AppendPoint( new FVectorPoint( iX, iY ) );

            mCubicPath->AppendPoint( new FVectorPointCubic( iX, iY, iRadius ), false, false );
        }
    }

    return NULL;
}
#endif

FVectorSegment*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY
                               , double iRadius )
{
    return AppendPoint ( iX, iY, iRadius, false );
}

FVectorPathCubic* 
FVectorPathBuilder::GetCubicPath()
{
    return mCubicPath;
}

FVectorSegment*
FVectorPathBuilder::End( double iX
                       , double iY
                       , double iRadius )
{
    FVectorPoint* lastPoint = GetLastPoint();

    if ( lastPoint->GetX() == iX && lastPoint->GetY() == iY )
    {
        // We delete the last point to prevent both points being at the same location
        // which would fake the result of AppendPoint() with iEnforce = true
        mPointList.remove( lastPoint );
    }

    return AppendPoint ( iX, iY, iRadius, true );
}

void
FVectorPathBuilder::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    BLPath path;

    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    iBLContext.setStrokeStyle( BLRgba32( mStrokeColor ) );
    iBLContext.setStrokeWidth( mStrokeWidth );

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment *segment = (*it);
        BLPoint point0;
        BLPoint point1;

        point0.x = segment->GetPoint(0)->GetX();
        point0.y = segment->GetPoint(0)->GetY();

        point1.x = segment->GetPoint(1)->GetX();
        point1.y = segment->GetPoint(1)->GetY();

        if( segment->GetPoint(0)->GetSegmentCount() == 1 )
        {
            path.moveTo( point0.x,point0.y );
        }

        path.lineTo( point1.x,point1.y );
    }

    iBLContext.strokePath( path );

    iBLContext.setFillStyle(BLRgba32(0xFFFF00FF));

    for( std::list<FVectorPoint*>::iterator it = mSamplePointList.begin(); it != mSamplePointList.end(); ++it )
    {
        FVectorPoint *samplePoint = (*it);

        iBLContext.fillRect( samplePoint->GetX() - 3, samplePoint->GetY() - 3, 6, 6  );
    }
}

bool
FVectorPathBuilder::PickPoint( double iX
                             , double iY
                             , double iRadius )
{
    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPoint* point = *it;

        if( ( fabs( point->GetX() - iX ) <= iRadius ) &&
            ( fabs( point->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back( point );

            return true;
        }
    }

    return false;
}

void
FVectorPathBuilder::Unselect(FVectorPoint *iPoint)
{
    if( iPoint == NULL )
    {
        mSelectedPointList.clear();
    } else
    {
        mSelectedPointList.remove(iPoint);
    }
}
