#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPointCubic* iPoint0
                                        , FVectorPointCubic* iPoint1 )
   : FVectorSegment( iPoint0, iPoint1 )
   , mPolygonSlot( 0 )
{
    mCtrlPoint[0].Set( iPoint0->GetX(), iPoint0->GetY() );
    mCtrlPoint[1].Set( iPoint1->GetX(), iPoint1->GetY() );

    mCtrlPoint[0].SetParent( this );
    mCtrlPoint[1].SetParent( this );
}

FVectorPointCubic&
FVectorSegmentCubic::GetPoint( int iPointNum )
{
    return static_cast<FVectorPointCubic&>( FVectorSegment::GetPoint( iPointNum ) );
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
    mPolygonSlot = 0;
}

FPolygon*
FVectorSegmentCubic::GetPolygonCacheSlot()
{
    return &mPolygonCache[mPolygonSlot++];
}

bool
FVectorSegmentCubic::Pick( double iX
                         , double iY
                         , double iRadius )
{
    for ( int i = 0; i < mPolygonSlot; i++ )
    {
        double refQuantity;
        bool collide = true;

        for ( int j = 0; j < 4; j++ )
        {
            int n = ( j + 1 ) % 4;
            FVec2D vivn = { mPolygonCache[i].vertex[n].x - mPolygonCache[i].vertex[j].x
                          , mPolygonCache[i].vertex[n].y - mPolygonCache[i].vertex[j].y };
            FVec2D vivt = { iX - mPolygonCache[i].vertex[j].x
                          , iY - mPolygonCache[i].vertex[j].y };
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
    FVec2D vec = { GetPoint(1).GetX() - GetControlPoint(1).GetX(),
                   GetPoint(1).GetY() - GetControlPoint(1).GetY() };

    if( iNormalize && vec.DistanceSquared() )
    {
        vec.Normalize();
    }

    return vec;
}

FVec2D FVectorSegmentCubic::GetVectorAtStart( bool iNormalize )
{
    FVec2D vec = { GetControlPoint(0).GetX() - GetPoint(0).GetX(),
                   GetControlPoint(0).GetY() - GetPoint(0).GetY() };

    if( iNormalize && vec.DistanceSquared() )
    {
        vec.Normalize();
    }

    return vec;
}

void
FVectorSegmentCubic::UpdateBoundingBox ()
{
   mBBox.x = ULIS::FMath::Min4<double>( mPoint[0]->GetX() + mPoint[0]->GetRadius()
                                      , mCtrlPoint[0].GetX()
                                      , mPoint[1]->GetX() + mPoint[1]->GetRadius()
                                      , mCtrlPoint[1].GetX() );

   mBBox.y = ULIS::FMath::Min4<double>( mPoint[0]->GetY() + mPoint[0]->GetRadius()
                                      , mCtrlPoint[0].GetY()
                                      , mPoint[1]->GetY() + mPoint[1]->GetRadius()
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

FVectorSegmentCubic::FVectorSegmentCubic( FVectorPointCubic* iPoint0
                                        , double iCtrlPoint0x
                                        , double iCtrlPoint0y
                                        , double iCtrlPoint1x
                                        , double iCtrlPoint1y
                                        , FVectorPointCubic* iPoint1 )
    : FVectorSegment( iPoint0, iPoint1 )
{
    mCtrlPoint[0].Set( iCtrlPoint0x, iCtrlPoint0y );
    mCtrlPoint[1].Set( iCtrlPoint1x, iCtrlPoint1y );
}

void
FVectorSegmentCubic::DrawStructure( FBlock& iBlock
                                  , BLContext& iBLContext
                                  , FRectD &iRoi )
{
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

    ctrlPath0.moveTo( point0.x, point0.y );
    ctrlPath0.lineTo( ctrlPoint0.x,ctrlPoint0.y );

    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF0000 ) );
    iBLContext.setStrokeWidth( 1.0f );

    iBLContext.strokePath( ctrlPath0 );

    ctrlPath1.moveTo( point1.x, point1.y );
    ctrlPath1.lineTo( ctrlPoint1.x,ctrlPoint1.y );

    iBLContext.strokePath( ctrlPath1 );

    iBLContext.setFillStyle( BLRgba32( 0xFFFF0000 ) );
    iBLContext.fillRect( ctrlPoint0.x - 2, ctrlPoint0.y - 2, 4, 4 );
    iBLContext.fillRect( ctrlPoint1.x - 2, ctrlPoint1.y - 2, 4, 4 );
}

void
FVectorSegmentCubic::Draw( FBlock& iBlock
                         , BLContext& iBLContext
                         , FRectD &iRoi )
{
    iBLContext.setStrokeWidth( 1.0f );

    for ( int i = 0; i < mPolygonSlot; i++ )
    {
       /* int n = i + 1;*/

        /*iBLContext.strokeLine( mPolygonCache[i].vertex[1], mPolygonCache[i].vertex[2] );*/

        // the stroke thing is very slow and slows the all thing, we have to find something better
        //iBLContext.strokePolygon( mPolygonCache[i].vertex, 4 );
        iBLContext.fillPolygon( mPolygonCache[i].vertex, 4 );
    }
}

void
FVectorSegmentCubic::BuildVariableThickness( double iFromT
                                           , double iToT
                                           , FVec2D& iFromPoint
                                           , FVec2D& iToPoint
                                           , FVec2D* iPrevSegmentVector
                                           , FVec2D* iNextSegmentVector
                                           , double iStartRadius
                                           , double iEndRadius )
{
    // TODO: use references for speed
    FVec2D& point0 = mPoint[0]->GetCoords();
    FVec2D& point1 = mPoint[1]->GetCoords();
    FVec2D& ctrlPoint0 = mCtrlPoint[0].GetCoords();
    FVec2D& ctrlPoint1 = mCtrlPoint[1].GetCoords();
    FVec2D sampleTangent[2] = { CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iFromT ),
                                CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iToT   ) };
    static FVec2D zeroVector = { 0.0f, 0.0f };

    if ( ( sampleTangent[0] != zeroVector ) &&  ( sampleTangent[1] != zeroVector ) )
    {
        // Note: no need to normalize sampleTangent[] vectors. The averaging won't be any different than with a normalized version.
        FVec2D parallelVecFrom[2] = { *iPrevSegmentVector, sampleTangent[0]    };
        FVec2D   parallelVecTo[2] = { sampleTangent[1]   , *iNextSegmentVector };
        FVec2D averageVecFrom = ( parallelVecFrom[0] + parallelVecFrom[1] ) * 0.5f;
        FVec2D averageVecTo   = (   parallelVecTo[0] +   parallelVecTo[1] ) * 0.5f;
        FVec2D perpendicularVecFrom = { averageVecFrom.y, -averageVecFrom.x };
        FVec2D perpendicularVecTo = { averageVecTo.y, -averageVecTo.x };
        uint32 prevPolygonSlot = mPolygonSlot;
        FPolygon* cachedPolygon = GetPolygonCacheSlot();

        if ( perpendicularVecFrom.DistanceSquared() && perpendicularVecTo.DistanceSquared() )
        {
            perpendicularVecFrom.Normalize();
            perpendicularVecTo.Normalize();
        }

        perpendicularVecFrom *= iStartRadius;
        perpendicularVecTo *= iEndRadius;

        if ( prevPolygonSlot ) {
            FPolygon* prevCachedPolygon = cachedPolygon - 1;

            cachedPolygon->vertex[0].x = prevCachedPolygon->vertex[1].x;
            cachedPolygon->vertex[0].y = prevCachedPolygon->vertex[1].y;

            cachedPolygon->vertex[1].x = iToPoint.x + perpendicularVecTo.x;
            cachedPolygon->vertex[1].y = iToPoint.y + perpendicularVecTo.y;

            cachedPolygon->vertex[2].x = iToPoint.x - perpendicularVecTo.x;
            cachedPolygon->vertex[2].y = iToPoint.y - perpendicularVecTo.y;

            cachedPolygon->vertex[3].x = prevCachedPolygon->vertex[2].x;
            cachedPolygon->vertex[3].y = prevCachedPolygon->vertex[2].y;
        }
        else
        {
            cachedPolygon->vertex[0].x = iFromPoint.x + perpendicularVecFrom.x;
            cachedPolygon->vertex[0].y = iFromPoint.y + perpendicularVecFrom.y;

            cachedPolygon->vertex[1].x = iToPoint.x + perpendicularVecTo.x;
            cachedPolygon->vertex[1].y = iToPoint.y + perpendicularVecTo.y;

            cachedPolygon->vertex[2].x = iToPoint.x - perpendicularVecTo.x;
            cachedPolygon->vertex[2].y = iToPoint.y - perpendicularVecTo.y;

            cachedPolygon->vertex[3].x = iFromPoint.x - perpendicularVecFrom.x;
            cachedPolygon->vertex[3].y = iFromPoint.y - perpendicularVecFrom.y;
        }
    }
}

void
FVectorSegmentCubic::BuildVariableAdaptive( double iFromT
                                          , double iToT
                                          , double iStartRadius
                                          , double iEndRadius
                                          , FVec2D* iPrevSegmentVector
                                          , FVec2D* iNextSegmentVector
                                          , int32 iMaxRecurseDepth )
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

        if( subTanDistanceSquared && subSegDistanceSquared )
        {
            // Note: must be normalized in order to get a correct value for the dot product
            subTangent[i].Normalize();
            subSegment[i].Normalize();

            if( subTangent[i].DotProduct( subSegment[i] ) <= angleCosineLimit )
            {
                doRefine = true;
            }
        }
    }

    for( int i = 0; i < 3; i++ )
    {
        int n = ( i + 1 );

        nextSegmentVector = ( i == 0x02 ) ? iNextSegmentVector : &subTangent[n];

        if( ( doRefine == true ) && --iMaxRecurseDepth >= 0 )
        {
            IncreasePolygonCache ( 3 );

            BuildVariableAdaptive( sampleRange[i]
                                 , sampleRange[n]
                                 , radius
                                 , radius + radiusStep
                                 , prevSegmentVector
                                 , nextSegmentVector
                                 , iMaxRecurseDepth );
        }
        else
        {
            BuildVariableThickness( sampleRange[i]
                                  , sampleRange[n]
                                  , samplePoint[i]
                                  , samplePoint[n]
                                  , prevSegmentVector
                                  , nextSegmentVector
                                  , radius
                                  , radius + radiusStep );
        }

       // Note: within the segment, the previous vector (at origin) is the segment's vector itself
        prevSegmentVector = &subTangent[n];

        radius += radiusStep;
    }
}

void
FVectorSegmentCubic::BuildVariable()
{
    double segmentStartRadius = static_cast<FVectorPointCubic*>(mPoint[0])->GetRadius();
    double segmentEndRadius = static_cast<FVectorPointCubic*>(mPoint[1])->GetRadius();
    static FVec2D zeroVector = { 0.0f, 0.0f };

    ResetPolygonCache();

    UpdateBoundingBox();

    // a segment is at least subdivided once anyways
    IncreasePolygonCache ( 3 );

    BuildVariableAdaptive ( 0.0f
                          , 1.0f
                          , segmentStartRadius
                          , segmentEndRadius
                          , &zeroVector
                          , &zeroVector
                          , 8 );
}