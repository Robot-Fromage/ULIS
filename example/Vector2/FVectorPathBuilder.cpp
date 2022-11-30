#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathBuilder::~FVectorPathBuilder()
{

}

FVectorPathBuilder::FVectorPathBuilder( FVectorPathCubic* iCubicPath )
    : mCubicPath ( iCubicPath )
    , mCumulAngle ( 0.0f )
    , mCumulAngleLimit ( 1.5708f ) // 90 degrees
    , mLastCubicAngleLimit ( 1.0472f ) // 60 deg 
    /*, mLastCubicAngleLimit ( 2.35619f )*/ // 135 degrees
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
FVectorPathBuilder::Sharp( FVectorSegmentCubic& iCubicSegment
                         , FVec2D iEntryVector
                         , FVec2D iExitVector )
{
    double length = iCubicSegment.GetVector( false ).Distance();

    iCubicSegment.GetControlPoint(0).Set( iCubicSegment.GetPoint(0)->GetX() + iEntryVector.x * length * 0.35f
                                        , iCubicSegment.GetPoint(0)->GetY() + iEntryVector.y * length * 0.35f );

    iCubicSegment.GetControlPoint(1).Set( iCubicSegment.GetPoint(1)->GetX() - iExitVector.x * length * 0.35f
                                        , iCubicSegment.GetPoint(1)->GetY() - iExitVector.y * length * 0.35f );

    iCubicSegment.Update();
}

FVectorSegmentCubic*
FVectorPathBuilder::Sample( FVectorPoint* iSamplePoint, double iRadius, bool iEnforce )
{
    FVectorPoint* lastSamplePoint = GetLastSamplePoint();
    FVectorSegmentCubic* lastCubicSegment = static_cast<FVectorSegmentCubic*>(mCubicPath->GetLastSegment());
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

            if ( ( angle > mLastCubicAngleLimit ) || ( mCumulAngle >= mCumulAngleLimit ) || iEnforce )
            {
                FVectorPointCubic* cubicPoint = new FVectorPointCubic( lastSamplePoint->GetX()
                                                                     , lastSamplePoint->GetY()
                                                                     , iRadius );
                FVec2D entryVector = mSampleSegmentList.front()->GetVector( true );
                FVec2D exitVector =  lastSampleSegmentVector;

                cubicSegment = mCubicPath->AppendPoint( cubicPoint, true, true );

                Sharp ( *cubicSegment
                        , entryVector
                        , exitVector );

                if ( lastCubicSegment )
                {
                    FVec2D lastCubicSegmentVector = lastCubicSegment->GetVectorAtEnd( true );
                    double smoothDot = entryVector.DotProduct( lastCubicSegmentVector );
                    double smoothAngle = acos( ULIS::FMath::Clamp<double>( smoothDot, -1.0f, 1.0f ) );

                    if ( fabs(smoothAngle) < mLastCubicAngleLimit )
                    {
                        static_cast<FVectorPointCubic*>(cubicSegment->GetPoint(0))->SmoothSegments( false );
                    }
                }

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

        if ( length >= 12.0f || iEnforce == true )
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

    mCubicPath->Update();

    return segment;
}

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
                       , double iRadius
                       , bool iClose )
{
    FVectorPoint* lastPoint = mCubicPath->GetLastPoint();
    FVectorPoint* firstPoint = mCubicPath->GetFirstPoint();
    FVectorSegment* firstSegment = mCubicPath->GetFirstSegment();
    FVectorSegmentCubic* newSegment = nullptr;;

    if ( lastPoint )
    {
        if ( lastPoint->GetX() == iX && lastPoint->GetY() == iY )
        {
            // We delete the last point to prevent both points being at the same location
            // which would fake the result of AppendPoint() with iEnforce = true
            mPointList.remove( lastPoint );
        }

        AppendPoint ( iX, iY, iRadius, true );

        newSegment = static_cast<FVectorSegmentCubic*>(mCubicPath->GetLastSegment());

        if ( iClose == true )
        {
            if ( firstSegment && newSegment )
            {
                FVectorPoint* newPoint = mCubicPath->GetLastPoint();

                if ( firstPoint->GetSegmentCount() == 1 )
                {
                    FVectorSegmentCubic* loopSegment = new FVectorSegmentCubic( *mCubicPath
                                                                               , static_cast<FVectorPointCubic*>(newPoint)
                                                                               , static_cast<FVectorPointCubic*>(firstPoint) );


                    mCubicPath->AddSegment( loopSegment );

                    /*Round( *loopSegment );*/
                }
            }
        }
    }

    return nullptr;
}

void
FVectorPathBuilder::DrawShape( FRectD& iRoi, uint64 iFlags )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    BLPath path;

    blctx.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    blctx.setStrokeStyle( BLRgba32( mStrokeColor ) );
    blctx.setStrokeWidth( mStrokeWidth );

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

    blctx.strokePath( path );

    blctx.setFillStyle(BLRgba32(0xFFFF00FF));

    for( std::list<FVectorPoint*>::iterator it = mSamplePointList.begin(); it != mSamplePointList.end(); ++it )
    {
        FVectorPoint *samplePoint = (*it);

        blctx.fillRect( samplePoint->GetX() - 3, samplePoint->GetY() - 3, 6, 6  );
    }
}

bool
FVectorPathBuilder::PickPoint( double iX
                             , double iY
                             , double iRadius
                             , uint64 iSelectionFlags )
{
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
