#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPathCubic& iPath
                                        , FVectorPointCubic* iPoint0
                                        , FVectorPointCubic* iPoint1 )
   : FVectorSegment( iPath, iPoint0, iPoint1 )
   , mCtrlPoint { FVectorHandleSegment( *this, iPoint0->GetX(), iPoint0->GetY() )
                , FVectorHandleSegment( *this, iPoint1->GetX(), iPoint1->GetY() ) }
{
    Update();
}

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPathCubic& iPath
                                        , FVectorPointCubic* iPoint0
                                        , double iCtrlPoint0x
                                        , double iCtrlPoint0y
                                        , double iCtrlPoint1x
                                        , double iCtrlPoint1y
                                        , FVectorPointCubic* iPoint1 )
    : FVectorSegment( iPath, iPoint0, iPoint1 )
    , mCtrlPoint { FVectorHandleSegment( *this, iCtrlPoint0x, iCtrlPoint0y )
                ,  FVectorHandleSegment( *this, iCtrlPoint1x, iCtrlPoint1y ) }
{
    Update();
}

FVec2D
FVectorSegmentCubic::GetPointAt( double t )
{
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    FVec2D pointAt = CubicBezierPointAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, t );

    return pointAt;
}

void
FVectorSegmentCubic::IncreasePolygonCache( uint32 iSize )
{
     mPolygonCache.resize( mPolygonCache.size() + iSize );
}

void
FVectorSegmentCubic::ResetPolygonCache( )
{
    mPolygonCache.clear();
}

bool
FVectorSegmentCubic::Pick( double iX
                         , double iY
                         , double iRadius )
{
    for ( int i = 0; i < mPolygonCache.size(); i++ )
    {
        double refQuantity;
        bool collide = true;

        for ( int j = 0; j < 4; j++ )
        {
            int n = ( j + 1 ) % 4;
            FVec2D vivn = { mPolygonCache[i].quadVertex[n].x - mPolygonCache[i].quadVertex[j].x
                          , mPolygonCache[i].quadVertex[n].y - mPolygonCache[i].quadVertex[j].y };
            FVec2D vivt = { iX - mPolygonCache[i].quadVertex[j].x
                          , iY - mPolygonCache[i].quadVertex[j].y };
            // https://stackoverflow.com/questions/15490795/determine-if-a-2d-point-is-within-a-quadrilateral
            // Compute the quantity
            double quantity = (vivt.x) * (vivn.y) - (vivn.x) * (vivt.y);

            if( j == 0 )
            {
                refQuantity = quantity;
            }

            if ( refQuantity * quantity < 0.0f )
            {
                collide = false;

                break;
            }
        }

        if ( collide == true ) return true;
    }

    return false;
}

FVec2D FVectorSegmentCubic::GetVectorAtEnd( bool iNormalize )
{
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    FVec2D vec = CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, 0.9999f );
/*
    FVec2D vec = { GetPoint(1)->GetX() - GetControlPoint(1).GetX(),
                   GetPoint(1)->GetY() - GetControlPoint(1).GetY() };
*/
    if( iNormalize && vec.DistanceSquared() )
    {
        vec.Normalize();
    }

    return vec;
}

FVec2D FVectorSegmentCubic::GetVectorAtStart( bool iNormalize )
{
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    FVec2D vec = CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, 0.0001f );
/*
    FVec2D vec = { GetControlPoint(0).GetX() - GetPoint(0)->GetX(),
                   GetControlPoint(0).GetY() - GetPoint(0)->GetY() };
*/
    if( iNormalize && vec.DistanceSquared() )
    {
        vec.Normalize();
    }

    return vec;
}

void
FVectorSegmentCubic::UpdateBoundingBox ()
{
   mBBox.x = ULIS::FMath::Min4<double>( mPoint[0]->GetX() - mPoint[0]->GetRadius()
                                      , mCtrlPoint[0].GetX()
                                      , mPoint[1]->GetX() - mPoint[1]->GetRadius()
                                      , mCtrlPoint[1].GetX() );

   mBBox.y = ULIS::FMath::Min4<double>( mPoint[0]->GetY() - mPoint[0]->GetRadius()
                                      , mCtrlPoint[0].GetY()
                                      , mPoint[1]->GetY() - mPoint[1]->GetRadius()
                                      , mCtrlPoint[1].GetY() );

   mBBox.w = ULIS::FMath::Max4<double>( mPoint[0]->GetX() + mPoint[0]->GetRadius()
                                      , mCtrlPoint[0].GetX()
                                      , mPoint[1]->GetX() + mPoint[1]->GetRadius()
                                      , mCtrlPoint[1].GetX() ) - mBBox.x;

   mBBox.h = ULIS::FMath::Max4<double>( mPoint[0]->GetY() + mPoint[0]->GetRadius()
                                      , mCtrlPoint[0].GetY()
                                      , mPoint[1]->GetY() + mPoint[1]->GetRadius()
                                      , mCtrlPoint[1].GetY() ) - mBBox.y;
}

FVectorHandleSegment&
FVectorSegmentCubic::GetControlPoint( int iCtrlPointNum )
{
    return mCtrlPoint[iCtrlPointNum];
}

FRectD&
FVectorSegmentCubic::GetBoundingBox( )
{
    return mBBox;
}


// https://stackoverflow.com/questions/35473936/find-whether-two-line-segments-intersect-or-not-in-c
bool intersection( FVec2D& line0p0
                 , FVec2D& line0p1
                 , FVec2D& line1p0
                 , FVec2D& line1p1
                 , double* line0t
                 , double* line1t )
{
    FVec2D L0Vec   = { line0p1.x - line0p0.x, line0p1.y - line0p0.y };
    FVec2D L1Vec   = { line1p0.x - line1p1.x, line1p0.y - line1p1.y }; // reverted order is normal
    FVec2D L0L1Vec = { line1p0.x - line0p0.x, line1p0.y - line0p0.y };

    double det = L0Vec.x * L1Vec.y - L0Vec.y * L1Vec.x;

    if ( fabs(det) == 0.0f ) return false;

    double r = ( L0L1Vec.x * L1Vec.y   - L0L1Vec.y * L1Vec.x   ) / det;
    double s = (   L0Vec.x * L0L1Vec.y -   L0Vec.y * L0L1Vec.x ) / det;

    *line0t = r;
    *line1t = s;

    return !(r < 0 || r > 1 || s < 0 || s > 1);
}

void
FVectorSegmentCubic::IntersectPath( FVectorPathCubic& iPath )
{
    std::list<FVectorSegment*>& segmentList = iPath.GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(*it);
        FRectD bbox = this->GetBoundingBox() & cubicSegment->GetBoundingBox();

        /*if ( bbox.Area() > 0.0f )
        {*/
           /* if ( cubicSegment != this )  // commented out: a bezier can intersect itself
            {*/
                Intersect( *cubicSegment ); 
           /* }*/
        /*}*/
    }
}

double
FVectorSegmentCubic::GetDistanceSquared()
{
    return mDistanceSquared;
}

bool
FVectorSegmentCubic::Cut( FVec2D& linePoint0
                        , FVec2D& linePoint1 )
{
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    // we'll have 3 intersections at most and 2 points at tips.
    FVectorPointCubic* pointChain[5] = { static_cast<FVectorPointCubic*>(mPoint[0]), nullptr, nullptr, nullptr, nullptr };
    FVec2D tangentChain[5] = { GetVectorAtStart( true ), { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f } };
    uint32 pointCount = 1;
    FVec2D ctrlPoint0Vector = GetVectorAtStart( true );
    FVec2D ctrlPoint1Vector = GetVectorAtEnd( true );

    for( int i = 0; i < mPolygonCache.size(); i++ )
    {
        FPolygon* poly = &mPolygonCache[i];
        double polySubT, interPolySubT;

        if ( intersection ( poly->lineVertex[0]
                          , poly->lineVertex[1]
                          , linePoint0
                          , linePoint1
                          , &polySubT
                          , &interPolySubT ) )
        {
            double segmentT = poly->fromT + ( polySubT * ( poly->toT - poly->fromT ) );
            FVec2D pointAt = CubicBezierPointAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, segmentT );
            FVectorPointCubic* newCubicPoint = new FVectorPointCubic( pointAt.x, pointAt.y );
            FVec2D newTangent = CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, segmentT );

            newCubicPoint->SetRadius( ( mPoint[0]->GetRadius() * segmentT ) + ( mPoint[1]->GetRadius() * ( 1.0f - segmentT ) ), false );

            if ( newTangent.DistanceSquared() )
            {
                newTangent.Normalize();
            }

            tangentChain[pointCount] = newTangent;
            pointChain[pointCount] = newCubicPoint;

            pointCount++;
        }
    }

    if ( pointCount > 1 )
    {
        tangentChain[pointCount] = GetVectorAtEnd( true );
        pointChain[pointCount] = static_cast<FVectorPointCubic*>(mPoint[1]);

        mPath.RemoveSegment( this );

        for( int i = 1; i < pointCount; i++ )
        {
            mPath.AddPoint( pointChain[i] );
        }

        for( int i = 0; i < pointCount; i++ )
        {
            uint32 n = i + 1;
            FVectorSegmentCubic* segment = new FVectorSegmentCubic ( static_cast<FVectorPathCubic&>(mPath)
                                                                   , static_cast<FVectorPointCubic*>(pointChain[i])
                                                                   , static_cast<FVectorPointCubic*>(pointChain[n]) );
            double distance = segment->GetStraightDistance();

            segment->GetControlPoint(0).Set( segment->GetPoint(0)->GetX() + ( tangentChain[i].x * distance * 0.35f )
                                           , segment->GetPoint(0)->GetY() + ( tangentChain[i].y * distance * 0.35f ) );

            segment->GetControlPoint(1).Set( segment->GetPoint(1)->GetX() - ( tangentChain[n].x * distance * 0.35f )
                                           , segment->GetPoint(1)->GetY() - ( tangentChain[n].y * distance * 0.35f ) );

            mPath.AddSegment( segment );
        }

        return true;
    }

    return false;
}

void
FVectorSegmentCubic::Intersect( FVectorSegmentCubic& iOther )
{
    FVec2D point0 = { mPoint[0]->GetX(), mPoint[0]->GetY() };
    FVec2D point1 = { mPoint[1]->GetX(), mPoint[1]->GetY() };
    FVec2D ctrlPoint0 = { mCtrlPoint[0].GetX(), mCtrlPoint[0].GetY() };
    FVec2D ctrlPoint1 = { mCtrlPoint[1].GetX(), mCtrlPoint[1].GetY() };

    std::list<FVectorPointIntersection*> intersectionPointList;

    // the other segment must not belong to this segment's points or we would get an intersection no matter what
    if ( ( mPoint[0]->HasSegment( iOther ) == false ) && 
         ( mPoint[1]->HasSegment( iOther ) == false ) )
    {
        for ( int i = 0; i < mPolygonCache.size(); i++ )
        {
            FPolygon* poly = &mPolygonCache[i];

            for( int j = 0; j < iOther.mPolygonCache.size(); j++ )
            {
                FPolygon* interPoly = &iOther.mPolygonCache[j];
                double polySubT, interPolySubT;

                if ( intersection ( poly->lineVertex[0]
                                  , poly->lineVertex[1]
                                  , interPoly->lineVertex[0]
                                  , interPoly->lineVertex[1]
                                  , &polySubT
                                  , &interPolySubT ) )
                {
                    FVec2D polyVector = ( poly->lineVertex[1] - poly->lineVertex[0] );
                    FVec2D coords = { poly->lineVertex[0].x + ( polyVector.x * polySubT )
                                    , poly->lineVertex[0].y + ( polyVector.y * polySubT ) };
                    FVectorPointIntersection* intersectionPoint = new FVectorPointIntersection ();

                    intersectionPointList.push_back( intersectionPoint );

                    double segmentT =      poly->fromT + (      polySubT * (      poly->toT -      poly->fromT ) );
                    double iOtherT  = interPoly->fromT + ( interPolySubT * ( interPoly->toT - interPoly->fromT ) );

    printf("new intersection Point: %d - %f %f\n", intersectionPoint, segmentT, iOtherT );

                    intersectionPoint->AddSegment (    this, segmentT );
                    intersectionPoint->AddSegment ( &iOther,  iOtherT );
                }
            }
        }

        if ( intersectionPointList.size() )
        {
/*
            this->ClearIntersections();
            iOther.ClearIntersections();
*/
            for( std::list<FVectorPointIntersection*>::iterator it = intersectionPointList.begin(); it != intersectionPointList.end(); ++it )
            {
                FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*it);

                this->AddIntersection ( intersectionPoint );
                iOther.AddIntersection ( intersectionPoint );

                intersectionPoint->March();
            }
        }
    }
}

void
FVectorSegmentCubic::DrawIntersections ( FRectD &iRoi
                                       , double iZoomFactor )
{
    BLContext& blctx = FVectorEngine::GetBLContext();
    double intersectionSize = 4 * iZoomFactor;
    double intersectionHalfSize = intersectionSize * 0.5f;
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();

    for( std::list<FVectorPointIntersection*>::iterator it = mIntersectionPointList.begin(); it != mIntersectionPointList.end(); ++it )
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*it);
        FVec2D pt = intersectionPoint->GetPosition(*this);

        blctx.setFillStyle( BLRgba32( 0xFFFF8000 ) );
        blctx.fillRect( pt.x - intersectionHalfSize
                      , pt.y - intersectionHalfSize, intersectionSize, intersectionSize );
    }
}

void
FVectorSegmentCubic::DrawStructure( FRectD &iRoi
                                  , double iZoomFactor )
{
    BLContext& blctx = FVectorEngine::GetBLContext();
    BLPath ctrlPath0;
    BLPath ctrlPath1;
    BLPoint point0 = { mPoint[0]->GetX(), mPoint[0]->GetY() };
    BLPoint point1 = { mPoint[1]->GetX(), mPoint[1]->GetY() };
    BLPoint ctrlPoint0 = { mCtrlPoint[0].GetX(), mCtrlPoint[0].GetY() };
    BLPoint ctrlPoint1 = { mCtrlPoint[1].GetX(), mCtrlPoint[1].GetY() };
    double handleSize = 4 * iZoomFactor;
    double handleHalfSize = handleSize * 0.5f;

    ctrlPath0.moveTo( point0.x, point0.y );
    ctrlPath0.lineTo( ctrlPoint0.x, ctrlPoint0.y );

    blctx.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );

    blctx.strokePath( ctrlPath0 );

    ctrlPath1.moveTo( point1.x, point1.y );
    ctrlPath1.lineTo( ctrlPoint1.x, ctrlPoint1.y );

    blctx.strokePath( ctrlPath1 );

    blctx.setFillStyle( BLRgba32( 0xFFFF0000 ) );
    blctx.fillRect( ctrlPoint0.x - handleHalfSize, ctrlPoint0.y - handleHalfSize, handleSize, handleSize );
    blctx.fillRect( ctrlPoint1.x - handleHalfSize, ctrlPoint1.y - handleHalfSize, handleSize, handleSize );

    DrawIntersections (  iRoi, iZoomFactor );
}

uint32
FVectorSegmentCubic::GetPolygonCount()
{
    return mPolygonCache.size();
}

std::vector<FPolygon>&
 FVectorSegmentCubic::GetPolygonCache()
{
    return mPolygonCache;
}

void
FVectorSegmentCubic::Draw( FRectD &iRoi )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    blctx.setStrokeWidth( 1.0f );

#ifdef UNUSED
    for ( int i = 0; i < mPolygonCache.size(); i++ )
    {
       /* int n = i + 1;*/

        // the stroke thing is very slow and slows the all thing, we have to find something better
        //iBLContext.strokePolygon( mPolygonCache[i].vertex, 4 );
        BLPoint pt[4] = { { mPolygonCache[i].quadVertex[0].x, mPolygonCache[i].quadVertex[0].y }
                        , { mPolygonCache[i].quadVertex[1].x, mPolygonCache[i].quadVertex[1].y }
                        , { mPolygonCache[i].quadVertex[2].x, mPolygonCache[i].quadVertex[2].y }
                        , { mPolygonCache[i].quadVertex[3].x, mPolygonCache[i].quadVertex[3].y } };

        /*iBLContext.strokeLine( pt[1].x
                             , pt[1].y
                             , pt[2].x
                             , pt[2].y );*/

        blctx.fillPolygon( pt, 4 );
    }
#endif
/*
#ifdef UNUSED
*/
    blctx.strokePath ( mBLPath );
    blctx.fillPath ( mBLPath );
/*
#endif
*/
}

void
FVectorSegmentCubic::BuildVariableThickness( double iFromT
                                           , double iToT
                                           , FVec2D& iFromPoint
                                           , FVec2D& iToPoint
                                           , FVec2D* iPrevSegmentVector
                                           , FVec2D* iNextSegmentVector
                                           , double iStartRadius
                                           , double iEndRadius
                                           , int    iPolygonID )
{
    // TODO: use references for speed
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    FVec2D sampleTangent[2] = { CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iFromT ),
                                CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, ( iToT == 1.0f ) ? 0.999f : iToT   ) };
    static FVec2D zeroVector = { 0.0f, 0.0f };
    FPolygon* cachedPolygon = &mPolygonCache[iPolygonID];

    // Note: no need to normalize sampleTangent[] vectors. The averaging won't be any different than with a normalized version.
    FVec2D parallelVecFrom[2] = { *iPrevSegmentVector, sampleTangent[0]    };
    FVec2D   parallelVecTo[2] = { sampleTangent[1]   , *iNextSegmentVector };
    FVec2D averageVecFrom = ( parallelVecFrom[0] + parallelVecFrom[1] ) * 0.5f;
    FVec2D averageVecTo   = (   parallelVecTo[0] +   parallelVecTo[1] ) * 0.5f;
    FVec2D perpendicularVecFrom = { averageVecFrom.y, -averageVecFrom.x };
    FVec2D perpendicularVecTo = { averageVecTo.y, -averageVecTo.x };
    /*int prevPolygonID = iPolygonID - 1;*/

    if ( perpendicularVecFrom.DistanceSquared() && perpendicularVecTo.DistanceSquared() )
    {
        perpendicularVecFrom.Normalize();
        perpendicularVecTo.Normalize();
    }

    perpendicularVecFrom *= iStartRadius;
    perpendicularVecTo *= iEndRadius;

    cachedPolygon->quadVertex[0].x = iFromPoint.x + perpendicularVecFrom.x;
    cachedPolygon->quadVertex[0].y = iFromPoint.y + perpendicularVecFrom.y;

    cachedPolygon->quadVertex[1].x = iToPoint.x + perpendicularVecTo.x;
    cachedPolygon->quadVertex[1].y = iToPoint.y + perpendicularVecTo.y;

    cachedPolygon->quadVertex[2].x = iToPoint.x - perpendicularVecTo.x;
    cachedPolygon->quadVertex[2].y = iToPoint.y - perpendicularVecTo.y;

    cachedPolygon->quadVertex[3].x = iFromPoint.x - perpendicularVecFrom.x;
    cachedPolygon->quadVertex[3].y = iFromPoint.y - perpendicularVecFrom.y;

    cachedPolygon->lineVertex[0].x = iFromPoint.x;
    cachedPolygon->lineVertex[0].y = iFromPoint.y;

    cachedPolygon->lineVertex[1].x = iToPoint.x;
    cachedPolygon->lineVertex[1].y = iToPoint.y;

    cachedPolygon->fromT = iFromT;
    cachedPolygon->toT = iToT;
/*
printf("cached polygon: %d - %f %f - %f %f - %f %f - %f %f - %f %f\n", iPolygonID
                                                                , cachedPolygon->quadVertex[0].x
                                                                , cachedPolygon->quadVertex[0].y
                                                                , cachedPolygon->quadVertex[1].x
                                                                , cachedPolygon->quadVertex[1].y
                                                                , cachedPolygon->quadVertex[2].x
                                                                , cachedPolygon->quadVertex[2].y
                                                                , cachedPolygon->quadVertex[3].x
                                                                , cachedPolygon->quadVertex[3].y
                                                                , cachedPolygon->fromT
                                                                , cachedPolygon->toT );
*/
}

void
FVectorSegmentCubic::BuildVariableAdaptive( double  iFromT
                                          , double  iToT
                                          , double  iStartRadius
                                          , double  iEndRadius
                                          , FVec2D* iPrevSegmentVector
                                          , FVec2D* iNextSegmentVector
                                          , int32   iMaxRecurseDepth
                                          , int    *iPolygonID )
{
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    double radiusDiff = iEndRadius - iStartRadius;
    double radiusStep = radiusDiff / 3;
    double rangeDiff = iToT - iFromT;
    double rangeStep = rangeDiff / 3;
    double sampleRange[4] = { iFromT, iFromT + rangeStep, iToT - rangeStep, iToT };
    FVec2D samplePoint[4] = { CubicBezierPointAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[0] ),
                              CubicBezierPointAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[1] ),
                              CubicBezierPointAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[2] ),
                              CubicBezierPointAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[3] ), };
    FVec2D subSegment[3] = { { samplePoint[1].x - samplePoint[0].x, samplePoint[1].y - samplePoint[0].y },
                             { samplePoint[2].x - samplePoint[1].x, samplePoint[2].y - samplePoint[1].y },
                             { samplePoint[3].x - samplePoint[2].x, samplePoint[3].y - samplePoint[2].y } };
    FVec2D subTangent[3] = { CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[0] ),
                             CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[1] ),
                             CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, sampleRange[2] ) };
    bool doRefine = false;
    double radius = iStartRadius;
    FVec2D *prevSegmentVector = iPrevSegmentVector;
    FVec2D *nextSegmentVector;
    double angleCosineLimit = 0.999f;
    static FVec2D nilVector = { 0.0f, 0.0f };

    for( int i = 0; i < 3; i++ )
    {
        int n = ( i + 1 );
        double subTanDistanceSquared = subTangent[i].DistanceSquared();
        double subSegDistanceSquared = subSegment[i].DistanceSquared();



        if( /*subTanDistanceSquared &&*/ subSegDistanceSquared )
        {
            // Note: must be normalized in order to get a correct value for the dot product
            if ( subTanDistanceSquared )
            {
                subTangent[i].Normalize();
            }

            subSegment[i].Normalize();

            if( subTangent[i].DotProduct( subSegment[i] ) <= angleCosineLimit )
            {
                doRefine = true;
            }
        }
    }

    --iMaxRecurseDepth;

    if( ( doRefine == true ) && iMaxRecurseDepth >= 0 )
    {
        for( int i = 0; i < 3; i++ )
        {
            int n = ( i + 1 );

            nextSegmentVector = ( i == 0x02 ) ? iNextSegmentVector : &subTangent[n];

            BuildVariableAdaptive( sampleRange[i]
                                 , sampleRange[n]
                                 , radius
                                 , radius + radiusStep
                                 , prevSegmentVector
                                 , nextSegmentVector
                                 , iMaxRecurseDepth
                                 , iPolygonID );

            // Note: within the segment, the previous vector (at origin) is the segment's vector itself
            prevSegmentVector = &subTangent[n];

            radius += radiusStep;
        }
    }
    else
    {
        IncreasePolygonCache (3);

        /*printf("increasing cache %d\n",mPolygonCache.size());*/

        for( int i = 0; i < 3; i++ )
        {
            int n = ( i + 1 );
            FVec2D dist = samplePoint[n] - samplePoint[i];

            mDistanceSquared += dist.DistanceSquared();

            nextSegmentVector = ( i == 0x02 ) ? iNextSegmentVector : &subTangent[n];

    /*printf("%f %f - %f %f\n", iFromT, iToT, sampleRange[i], sampleRange[n] );*/
                BuildVariableThickness( sampleRange[i]
                                      , sampleRange[n]
                                      , samplePoint[i]
                                      , samplePoint[n]
                                      , prevSegmentVector
                                      , nextSegmentVector
                                      , radius
                                      , radius + radiusStep
                                      , *iPolygonID );
    /*printf("%f %f\n", mPolygonCache[(*iPolygonID)].fromT, mPolygonCache[(*iPolygonID)].toT );*/
                (*iPolygonID)++; // insures polygons are ordered

            // Note: within the segment, the previous vector (at origin) is the segment's vector itself
            prevSegmentVector = &subTangent[n];

            radius += radiusStep;
        }
    }
}

void
FVectorSegmentCubic::Update()
{
    BuildVariable();
}

void
FVectorSegmentCubic::BuildVariable()
{
    double segmentStartRadius = static_cast<FVectorPointCubic*>(mPoint[0])->GetRadius();
    double segmentEndRadius = static_cast<FVectorPointCubic*>(mPoint[1])->GetRadius();
    static FVec2D zeroVector = { 0.0f, 0.0f };
    int polygonID = 0;

    mDistanceSquared = 0.0f;

    ResetPolygonCache();

    UpdateBoundingBox();

/*printf("\n");*/
    BuildVariableAdaptive ( 0.0f
                          , 1.0f
                          , segmentStartRadius
                          , segmentEndRadius
                          , &zeroVector
                          , &zeroVector
                          , 8
                          , &polygonID );

    if ( mPolygonCache.size() )
    {
        mBLPath.clear();

        mBLPath.moveTo ( mPolygonCache[0].quadVertex[0].x
                       , mPolygonCache[0].quadVertex[0].y );

        for ( int i = 0; i < mPolygonCache.size(); i++ )
        {
            mBLPath.lineTo ( mPolygonCache[i].quadVertex[1].x
                           , mPolygonCache[i].quadVertex[1].y );
        }

        for ( int i = mPolygonCache.size(); --i >= 0; )
        {
            mBLPath.lineTo ( mPolygonCache[i].quadVertex[2].x
                           , mPolygonCache[i].quadVertex[2].y );
        }

        mBLPath.lineTo ( mPolygonCache[0].quadVertex[3].x
                       , mPolygonCache[0].quadVertex[3].y );

        mBLPath.close();
    }
}