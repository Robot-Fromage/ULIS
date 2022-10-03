#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathBuilder::~FVectorPathBuilder()
{

}

FVectorPathBuilder::FVectorPathBuilder()
    : FVectorPath()
{
    mCubicPath = new FVectorPathCubic();
    mLastSmoothedSegmentVector.x = 0.0f;
    mLastSmoothedSegmentVector.y = 0.0f;
    mLastSmoothedSegmentVectorDistance = 0.0f;
    mLastSmoothedAngle = 0.0f;
}

FVectorSegment*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY
                               , bool   iEnforce )
{
    FVectorPoint* lastPoint = GetLastPoint();

    // lastPoint is NULL if this is the first point added
    if( lastPoint )
    {
        // remember the last segment before it gets changed by the call to the base AddSegment();
        FVectorSegment* lastSegment = GetLastSegment() /*mLastSegment*/;
        double difX = ( iX - lastPoint->GetX() );
        double difY = ( iY - lastPoint->GetY() );
        double distanceSq = ( difX * difX ) + ( difY * difY );

        /*** do not pick to many input points ***/
        if ( ( distanceSq > 144 ) || iEnforce == true )
        {
            FVec2D segVector( iX - lastPoint->GetX()
                            , iY - lastPoint->GetY() );
            double segVectorDistance = segVector.Distance();

            if( segVectorDistance )
            {
                FVectorPoint* iPoint = new FVectorPoint( iX, iY );
                FVectorSegment* segment = new FVectorSegment( lastPoint, iPoint );

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
                            FVectorPoint* lastSmoothedPoint = mCubicPath->GetLastPoint();
                            FVectorSegmentCubic* lastCubicSegment =  static_cast<FVectorSegmentCubic*>( mCubicPath->GetLastSegment() );
                            FVectorSegmentCubic* cubicSegment = mCubicPath->AppendPoint( new FVectorPoint( lastPoint->GetX()
                                                                                                         , lastPoint->GetY() ) );

                            double cubicSegmentStraightDistance = cubicSegment->GetStraightDistance();
                            // Note: lastSegVector is now normalized

                            FVec2D ctrlPt0 = { lastSmoothedPoint->GetX() + mLastSmoothedSegmentVector.x * cubicSegmentStraightDistance * 0.3f
                                             , lastSmoothedPoint->GetY() + mLastSmoothedSegmentVector.y * cubicSegmentStraightDistance * 0.3f };
                            FVec2D ctrlPt1 = { lastPoint->GetX() - lastSegVector.x * cubicSegmentStraightDistance * 0.3f
                                             , lastPoint->GetY() - lastSegVector.y * cubicSegmentStraightDistance * 0.3f };

                            cubicSegment->GetControlPoint( 0 )->SetX( ctrlPt0.x );
                            cubicSegment->GetControlPoint( 0 )->SetY( ctrlPt0.y );
                            cubicSegment->GetControlPoint( 1 )->SetX( ctrlPt1.x );
                            cubicSegment->GetControlPoint( 1 )->SetY( ctrlPt1.y );

                            if( lastCubicSegment )
                            {
                                double mRoundedAngle = acos(ULIS::FMath::Clamp<double>(mLastSmoothedSegmentVector.DotProduct(lastSegVector),-1.0f,1.0f)) / 3.14159 * 180;

                                if( mLastSmoothedAngle < 45.0f )
                                {
                                    double lastCubicSegmentStraightDistance = lastCubicSegment->GetStraightDistance();

                                    FVec2D vec = { lastCubicSegment->GetPoint(1)->GetX() - lastCubicSegment->GetControlPoint(1)->GetX(),
                                                   lastCubicSegment->GetPoint(1)->GetY() - lastCubicSegment->GetControlPoint(1)->GetY() };

                                    vec.Normalize();

                                    // uncomment to round based on average vectors
                                    vec = ( vec + mLastSmoothedSegmentVector ) * 0.5f;

                                    FVec2D ctrlPtcurr = { lastSmoothedPoint->GetX() + vec.x * cubicSegmentStraightDistance     * 0.3f
                                                        , lastSmoothedPoint->GetY() + vec.y * cubicSegmentStraightDistance     * 0.3f };
                                    FVec2D ctrlPtprev = { lastSmoothedPoint->GetX() - vec.x * lastCubicSegmentStraightDistance * 0.3f
                                                        , lastSmoothedPoint->GetY() - vec.y * lastCubicSegmentStraightDistance * 0.3f };

                                    lastCubicSegment->GetControlPoint( 1 )->Set( ctrlPtprev.x, ctrlPtprev.y );
                                        cubicSegment->GetControlPoint( 0 )->Set( ctrlPtcurr.x, ctrlPtcurr.y );
                                }
                                /*else
                                {
                                    cubicSegment->GetControlPoint(0)->SetX(lastSmoothedPoint->GetX());
                                    cubicSegment->GetControlPoint(0)->SetY(lastSmoothedPoint->GetY());
                                }*/
                            }

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

        mCubicPath->AppendPoint( new FVectorPoint( iX, iY ) );
    }

    return NULL;
}

FVectorSegment*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY )
{
    return AppendPoint ( iX, iY, false );
}

FVectorPathCubic* 
FVectorPathBuilder::GetSmoothedPath()
{
    return mCubicPath;
}

FVectorSegment*
FVectorPathBuilder::Close( double iX
                         , double iY )
{
    return AppendPoint ( iX, iY, true );
}

void
FVectorPathBuilder::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{
    BLPath path;

    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    iBLContext.setStrokeStyle( BLRgba32( mStrokeColor ) );
    iBLContext.setStrokeWidth( mStrokeWidth );

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);
        BLPoint point0;
        BLPoint point1;

        point0.x = segment->GetPoint(0)->GetX();
        point0.y = segment->GetPoint(0)->GetY();

        point1.x = segment->GetPoint(1)->GetX();
        point1.y = segment->GetPoint(1)->GetY();

        iBLContext.setStrokeStyle( BLRgba32(0xFF000000) );

        if( segment->GetPoint(0)->GetSegmentCount() == 1 )
        {
            path.moveTo( point0.x,point0.y );
        }

        path.lineTo( point1.x,point1.y );
    }
    iBLContext.strokePath( path );

    mCubicPath->DrawShape( iBlock, iBLContext );
}

void
FVectorPathBuilder::Pick( double iX
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
        }
    }
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
