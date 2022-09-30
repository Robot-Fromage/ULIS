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
}

FVectorSegment*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY
                               , bool   iEnforce )
{
    // lastPoint is NULL if this is the first point added
    if( mLastPoint )
    {
        // remember the last segment before it gets changed by the call to the base AddSegment();
        FVectorSegment* lastSegment = mLastSegment;
        double difX = ( iX - mLastPoint->GetX() );
        double difY = ( iY - mLastPoint->GetY() );
        double distanceSq = ( difX * difX ) + ( difY * difY );

        /*** do not pick to many input points ***/
        if ( ( distanceSq > 64 ) || iEnforce == true )
        {
            FVec2D segVector( iX - mLastPoint->GetX()
                            , iY - mLastPoint->GetY() );
            double segVectorDistance = segVector.Distance();

            if( segVectorDistance )
            {
                FVectorPoint* lastPoint = mLastPoint;
                FVectorPoint* iPoint = new FVectorPoint( iX, iY );
                FVectorSegment* segment = new FVectorSegment( mLastPoint, iPoint );

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
                    static double angle = 0.0f;
                    static double angleLimit = 90.0f;

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
                        lastSegVector.Normalize();

                        angle += acos(ULIS::FMath::Clamp<double>(segVector.DotProduct( lastSegVector ), -1.0f, 1.0f)) / 3.14159 * 180;

                        double ratio = lastSegVector.Distance() / segVector.Distance();

                        if( ( angle > angleLimit ) || iEnforce == true )
                        {
                            FVectorPoint* lastSmoothedPoint = mCubicPath->GetLastPoint();
                            FVectorSegmentCubic* cubicSegment = mCubicPath->AppendPoint( new FVectorPoint( lastPoint->GetX()
                                                                                                         , lastPoint->GetY() ) );

                            double straightDistance = cubicSegment->GetStraightDistance();
                            // Note: lastSegVector is now normalized
        printf("adding %f\n", straightDistance);
                            FVec2D ctrlPt0 = { lastSmoothedPoint->GetX() + mLastSmoothedSegmentVector.x * straightDistance * 0.5f
                                             , lastSmoothedPoint->GetY() + mLastSmoothedSegmentVector.y * straightDistance * 0.5f };
                            FVec2D ctrlPt1 = { lastPoint->GetX() - lastSegVector.x * straightDistance * 0.5f
                                             , lastPoint->GetY() - lastSegVector.y * straightDistance * 0.5f };

                            cubicSegment->GetControlPoint(0)->SetX( ctrlPt0.x );
                            cubicSegment->GetControlPoint(0)->SetY( ctrlPt0.y );
                            cubicSegment->GetControlPoint(1)->SetX( ctrlPt1.x );
                            cubicSegment->GetControlPoint(1)->SetY( ctrlPt1.y );

                            mSmoothedPointList.push_back( mLastPoint );

                            mLastSmoothedSegmentVector.x = segVector.x;
                            mLastSmoothedSegmentVector.y = segVector.y;
                            mLastSmoothedSegmentVectorDistance = segVectorDistance;

                            angle = 0;
                        }

        /*
                    lastPoint->Set( ( lastSegmentTotalX + segmentTotalX ) * 0.25f
                                    , ( lastSegmentTotalY + segmentTotalY ) * 0.25f );
        */

        /*
                    double newX = ( lastSegmentTotalX + segmentTotalX ) * 0.25f
                            , newY = ( lastSegmentTotalY + segmentTotalY ) * 0.25f;
        */
                        /*mSmoothedPointList.push_back( new FVectorPoint( lastSegmentTotalX * 0.5f, lastSegmentTotalY * 0.5f ) );
                        mSmoothedPointList.push_back( new FVectorPoint(     segmentTotalX * 0.5f,     segmentTotalY * 0.5f ) );*/
                    }
                }

                mLastSegment = segment;

                return segment;
            }
        }
    }
    else
    {
        mLastPoint = new FVectorPoint( iX, iY );

        mCubicPath->AppendPoint( new FVectorPoint( mLastPoint->GetX()
                                                 , mLastPoint->GetY() ) );
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
    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    mCubicPath->DrawShape( iBlock, iBLContext );

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->Draw( iBlock, iBLContext );
    }

    for(std::list<FVectorPoint*>::iterator it = mSmoothedPointList.begin(); it != mSmoothedPointList.end(); ++it)
    {
        FVectorPoint *point = (*it);

        iBLContext.setFillStyle(BLRgba32(0xFF0000FF));
        iBLContext.fillRect( point->GetX(), point->GetY(), 1, 1);
/*
        point->Draw(iBlock,iBLContext);
*/
    }
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
