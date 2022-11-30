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

FVectorLink*
FVectorPathBuilder::GetLastSampleLink()
{
    if( mSampleLinkList.size() == 0 ) return nullptr;

    return mSampleLinkList.back();
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
        FVectorLink* lastSampleLink = GetLastSampleLink();
        FVectorLink* sampleLink = new FVectorLink ( lastSamplePoint, iSamplePoint );

        mSampleLinkList.push_back( sampleLink );

        if ( lastSampleLink )
        {
            FVec2D lastSampleSegmentVector = lastSampleLink->GetVector( true );
            FVec2D     sampleSegmentVector =     sampleLink->GetVector( true );
            double dot = lastSampleSegmentVector.DotProduct( sampleSegmentVector );
            double angle = acos( ULIS::FMath::Clamp<double>( dot, -1.0f, 1.0f ) );

            mCumulAngle += angle;

            if ( ( angle > mLastCubicAngleLimit ) || ( mCumulAngle >= mCumulAngleLimit ) || iEnforce )
            {
                FVectorPointCubic* cubicPoint = new FVectorPointCubic( lastSamplePoint->GetX()
                                                                     , lastSamplePoint->GetY()
                                                                     , iRadius );
                FVec2D entryVector = mSampleLinkList.front()->GetVector( true );
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

static void
ClearUntil( std::list<FVectorPoint*>& iPointList
          , std::list<FVectorPoint*>& iSamplePointList
          , std::list<FVectorLink*>& iLinkList
          , std::list<FVectorLink*>& iSampleLinkList
          , FVectorPoint* iPoint )
{
    while ( iPointList.size() && ( iPointList.front() != iPoint ) )
    {
        iPointList.pop_front();
    }

    while ( iSamplePointList.size() && ( iSamplePointList.front() != iPoint ) )
    {
        iSamplePointList.pop_front();
    }

    while ( iLinkList.size() && ( iLinkList.front()->GetPoint(0) != iPoint ) )
    {
        iLinkList.pop_front();
    }

    while ( iSampleLinkList.size() && ( iSampleLinkList.front()->GetPoint(0) != iPoint ) )
    {
        iSampleLinkList.pop_front();
    }
}

FVectorSegmentCubic*
FVectorPathBuilder::AppendPoint( double iX
                               , double iY
                               , double iRadius
                               , bool   iEnforce )
{
    FVectorPoint* point = new FVectorPoint( iX, iY );
    FVectorPoint* lastPoint = ( mPointList.size() ) ? mPointList.back() : nullptr;
    FVectorSegmentCubic* cubicSegment = nullptr;

    mPointList.push_back( point );

    if ( lastPoint == nullptr )
    {
        FVectorPointCubic* cubicPoint = new FVectorPointCubic( point->GetX()
                                                             , point->GetY()
                                                             , iRadius );
        FVectorPoint* samplePoint = new FVectorPoint ( point->GetX(), point->GetY() );

        mCubicPath->AppendPoint( cubicPoint, false, false );

        Sample ( samplePoint, iRadius, iEnforce );
    }
    else
    {
        FVectorLink* link = new FVectorLink ( lastPoint, point );
        FVectorPoint* lastSamplePoint = GetLastSamplePoint();
        FVec2D dif = { iX - lastSamplePoint->GetX()
                     , iY - lastSamplePoint->GetY() };
        double length = dif.Distance();

        if ( length >= 12.0f || iEnforce == true )
        {
            cubicSegment = Sample ( point, iRadius, iEnforce );

            if ( cubicSegment )
            {
                /*FitSegment( *cubicSegment, mSampleLinkList );*/

                ClearUntil ( mPointList, mSamplePointList, mLinkList, mSampleLinkList, lastSamplePoint );
            }
        }

        mLinkList.push_back ( link );
    }

    mCubicPath->Update();

    return cubicSegment;
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
FVectorPathBuilder::FitSegment( FVectorSegmentCubic& iSegment
                              , std::list<FVectorLink*>& iLinkList )
{
    FVec2D& point0 = iSegment.GetPoint(0)->GetCoords();
    FVec2D& point1 = iSegment.GetPoint(1)->GetCoords();
    FVec2D& ctrlPoint0 = iSegment.GetControlPoint(0).GetCoords();
    FVec2D& ctrlPoint1 = iSegment.GetControlPoint(1).GetCoords();
    double sampleLength = 0.0f;
    double currentT = 0.0f;

    for( std::list<FVectorLink*>::iterator it = iLinkList.begin(); it != iLinkList.end(); ++it )
    {
        FVectorLink *link = (*it);

        sampleLength += link->GetStraightDistance();
    }

    if ( sampleLength )
    {
        double A1 = 0.0f, A2 = 0.0f, A12 = 0.0f;
        FVec2D C1 = { 0.0f, 0.0f };
        FVec2D C2 = { 0.0f, 0.0f };

        for( std::list<FVectorLink*>::iterator it = iLinkList.begin(); it != iLinkList.end(); ++it )
        {
            FVectorLink *link = (*it);
            double t = currentT + ( link->GetStraightDistance() / sampleLength );
            double ct = 1.0f - t;
            double t3 = pow ( t, 3 );
            double ct3 = pow ( ct, 3 );
            FVec2D PC = { link->GetPoint(1)->GetX() - ( ct3 * point0.x ) - ( t3 * point1.x )
                        , link->GetPoint(1)->GetY() - ( ct3 * point0.y ) - ( t3 * point1.y ) };

            A1  += ( pow ( t, 2 ) * pow ( ( ct ), 4 ) );
            A2  += ( pow ( t, 4 ) * pow ( ( ct ), 2 ) );
            A12 += ( pow ( t, 3 ) * pow ( ( ct ), 3 ) );

            C1.x += ( 3 * t * pow ( ct, 2 ) * PC.x );
            C1.y += ( 3 * t * pow ( ct, 2 ) * PC.y );

            C2.x += ( 3 * pow ( t, 2 ) * ct * PC.x );
            C2.y += ( 3 * pow ( t, 2 ) * ct * PC.y );

            currentT = t;
        }

        A1  *= 9.0f;
        A2  *= 9.0f;
        A12 *= 9.0f;

        ctrlPoint0.x = ( A2 * C1.x - A12 * C2.x ) / ( A1 * A2 - A12 * A12 );
        ctrlPoint0.y = ( A2 * C1.y - A12 * C2.y ) / ( A1 * A2 - A12 * A12 );

        ctrlPoint1.x = ( A1 * C2.x - A12 * C1.x ) / ( A1 * A2 - A12 * A12 );
        ctrlPoint1.y = ( A1 * C2.y - A12 * C1.y ) / ( A1 * A2 - A12 * A12 );
    }
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

    for( std::list<FVectorLink*>::iterator it = mLinkList.begin(); it != mLinkList.end(); ++it )
    {
        FVectorLink *link = (*it);
        BLPoint point0;
        BLPoint point1;

        point0.x = link->GetPoint(0)->GetX();
        point0.y = link->GetPoint(0)->GetY();

        point1.x = link->GetPoint(1)->GetX();
        point1.y = link->GetPoint(1)->GetY();

        path.moveTo( point0.x,point0.y );
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
