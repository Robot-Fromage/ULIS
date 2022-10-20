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

    FVectorPath::AppendPoint( iPoint );

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
                           , double iSelectionRadius )
{
    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {
        FVectorPointCubic* point = static_cast<FVectorPointCubic*>(*it);
        FVec2D perpendicularVector = point->GetPerpendicularVector( true );

        if( ( fabs( point->GetX() - iX ) <= iSelectionRadius ) &&
            ( fabs( point->GetY() - iY ) <= iSelectionRadius ) )
        {
            mSelectedPointList.push_back(point);
        }

        if( ( fabs( point->GetX() + ( perpendicularVector.x * point->GetRadius() ) - iX ) <= iSelectionRadius ) &&
            ( fabs( point->GetY() + ( perpendicularVector.y * point->GetRadius() ) - iY ) <= iSelectionRadius ) )
        {
            mSelectedPointList.push_back( &point->GetControlPoint() );
        }
    }

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPoint& ctrlPoint0 = segment->GetControlPoint( 0 );
        FVectorPoint& ctrlPoint1 = segment->GetControlPoint( 1 );

        if( ( fabs( ctrlPoint0.GetX() - iX ) <= iSelectionRadius ) &&
            ( fabs( ctrlPoint0.GetY() - iY ) <= iSelectionRadius ) )
        {
            mSelectedPointList.push_back( &ctrlPoint0 );
        }

        if( ( fabs( ctrlPoint1.GetX() - iX ) <= iSelectionRadius ) &&
            ( fabs( ctrlPoint1.GetY() - iY ) <= iSelectionRadius ) )
        {
            mSelectedPointList.push_back( &ctrlPoint1 );
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

        point0.x = segment->GetPoint( 0 ).GetX();
        point0.y = segment->GetPoint( 0 ).GetY();

        ctrlPoint0.x = segment->GetControlPoint( 0 ).GetX();
        ctrlPoint0.y = segment->GetControlPoint( 0 ).GetY();

        ctrlPoint1.x = segment->GetControlPoint( 1 ).GetX();
        ctrlPoint1.y = segment->GetControlPoint( 1 ).GetY();

        point1.x = segment->GetPoint( 1 ).GetX();
        point1.y = segment->GetPoint( 1 ).GetY();

        if ( segment->GetPoint( 0 ).GetSegmentCount() == 1 )
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
        FVectorPointCubic *point = static_cast<FVectorPointCubic*>(*it);
        FVec2D perpendicular = point->GetPerpendicularVector( true );
        double pointRadius = point->GetRadius();
        double ctrlX = point->GetX() + ( perpendicular.x * pointRadius );
        double ctrlY = point->GetY() + ( perpendicular.y * pointRadius );

        iBLContext.setFillStyle( BLRgba32( 0xFFFF00FF ) );
        iBLContext.fillRect( point->GetX() - 3, point->GetY() - 3, 6, 6 );

        iBLContext.setFillStyle( BLRgba32( 0xFF808080 ) );
        iBLContext.fillRect( ctrlX - 3, ctrlY - 3, 6, 6 );
    }
}

void
FVectorPathCubic::DrawSegment( BLPath& iPath
                             , FVectorSegmentCubic& iSegment
                             , bool    iIsStandalone )
{
    BLPoint point0;
    BLPoint point1;
    BLPoint ctrlPoint0;
    BLPoint ctrlPoint1;

    point0.x = iSegment.GetPoint(0).GetX();
    point0.y = iSegment.GetPoint(0).GetY();

    ctrlPoint0.x = iSegment.GetControlPoint(0).GetX();
    ctrlPoint0.y = iSegment.GetControlPoint(0).GetY();

    ctrlPoint1.x = iSegment.GetControlPoint(1).GetX();
    ctrlPoint1.y = iSegment.GetControlPoint(1).GetY();

    point1.x = iSegment.GetPoint(1).GetX();
    point1.y = iSegment.GetPoint(1).GetY();

    if ( iIsStandalone == true )
    {
        iPath.moveTo( point0.x, point0.y );
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
    iBLContext.setStrokeWidth( 20 );

    path.clear();

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
        bool isStandalone = ( segment->GetPoint(0).GetSegmentCount() == 1 );

        DrawSegment( path, *segment, isStandalone );
    }

    iBLContext.strokePath( path );
*/
    DrawShapeVariable( iBlock, iBLContext );

    if( mIsSelected )
    {
        DrawStructure( iBlock, iBLContext );
    }
}

void
FVectorPathCubic::DrawSegmentVariableThickness( FBlock& iBlock
                                              , BLContext& iBLContext
                                              , FVectorSegmentCubic& iSegment
                                              , double iFromT
                                              , double iToT
                                              , FVec2D& iFromPoint // that way we dont have to query the cubic curve at point0 and point1
                                              , FVec2D& iToPoint // that way we dont have to query the cubic curve at point0 and point1
                                              , FVec2D* iPrevSegmentVector
                                              , FVec2D* iNextSegmentVector
                                              , double iStartRadius
                                              , double iEndRadius )
{
    // TODO: use references for speed
    FVec2D& point0 = iSegment.GetPoint(0).GetCoords();
    FVec2D& point1 = iSegment.GetPoint(1).GetCoords();
    FVec2D& ctrlPoint0 = iSegment.GetControlPoint(0).GetCoords();
    FVec2D& ctrlPoint1 = iSegment.GetControlPoint(1).GetCoords();
    // prepare sample points to compute average perpendicular vectors
    double rangeDiff = iToT - iFromT;
    double rangeStep = rangeDiff / 10;
    double fromTminus = iFromT - rangeStep;
    double fromTplus = iFromT + rangeStep;
    double toTminus = iToT - rangeStep;
    double toTplus = iToT + rangeStep;
    FVec2D sampleTangent[2] = { CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iFromT ),
                                CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iToT   ) };

    sampleTangent[0].Normalize();
    sampleTangent[1].Normalize();

    FVec2D parallelVecFrom[2] = { *iPrevSegmentVector, sampleTangent[0]    };
    FVec2D   parallelVecTo[2] = { sampleTangent[1]   , *iNextSegmentVector };
    FVec2D averageVecFrom = ( parallelVecFrom[0] + parallelVecFrom[1] );
    FVec2D averageVecTo = ( parallelVecTo[0] + parallelVecTo[1] );

    averageVecFrom.Normalize();
    averageVecTo.Normalize();

    FVec2D perpendicularVecFrom = { averageVecFrom.y, -averageVecFrom.x };
    FVec2D perpendicularVecTo = { averageVecTo.y, -averageVecTo.x };
    /*BLPath path;*/
    BLPoint vertex[4];

    //printf("%f %f\n", parallelVecFrom[0].DotProduct(parallelVecFrom[1]), parallelVecTo[0].DotProduct(parallelVecTo[1]) );

    /*if( parallelVecFrom[0].DotProduct(parallelVecFrom[1]) != 0.0f && parallelVecTo[0].DotProduct(parallelVecTo[1]) != 0.0f )
    {*/
        /*if(perpendicularVecFrom.DistanceSquared())
        {
            perpendicularVecFrom.Normalize();
        }

        if(perpendicularVecTo.DistanceSquared())
        {
            perpendicularVecTo.Normalize();
        }*/

        vertex[0].x = iFromPoint.x + (perpendicularVecFrom.x * iStartRadius);
        vertex[0].y = iFromPoint.y + (perpendicularVecFrom.y * iStartRadius);

        vertex[1].x = iToPoint.x + (perpendicularVecTo.x * iEndRadius);
        vertex[1].y = iToPoint.y + (perpendicularVecTo.y * iEndRadius);

        vertex[2].x = iToPoint.x - (perpendicularVecTo.x * iEndRadius);
        vertex[2].y = iToPoint.y - (perpendicularVecTo.y * iEndRadius);

        vertex[3].x = iFromPoint.x - (perpendicularVecFrom.x * iStartRadius);
        vertex[3].y = iFromPoint.y - (perpendicularVecFrom.y * iStartRadius);

        iBLContext.strokePolygon(vertex , 4);
        /*iBLContext.fillPolygon(vertex , 4);*/

/*
        vertex[0].x = iToPoint.x; //+ (perpendicularVecFrom.x * iStartRadius);
        vertex[0].y = iToPoint.y; //+ (perpendicularVecFrom.y * iStartRadius);

        vertex[1].x = iFromPoint.x //+ (perpendicularVecTo.x * iEndRadius);
        vertex[1].y = iFromPoint.y //+ (perpendicularVecTo.y * iEndRadius);

        vertex[2].x = iFromPoint.x + (perpendicularVecFrom.x * iStartRadius);
        vertex[2].y = iFromPoint.y + (perpendicularVecFrom.y * iStartRadius);

        vertex[3].x = iToPoint.x + (perpendicularVecTo.x * iEndRadius);
        vertex[3].y = iToPoint.y + (perpendicularVecTo.y * iEndRadius);

        iBLContext.fillPolygon(vertex,4);
*/
    /*}*/
}

void
FVectorPathCubic::DrawSegmentVariable( FBlock& iBlock
                                     , BLContext& iBLContext
                                     , FVectorSegmentCubic& iSegment
                                     , double iFromT
                                     , double iToT
                                     , double iStartRadius
                                     , double iEndRadius
                                     , FVec2D* iPrevSegmentVector
                                     , FVec2D* iNextSegmentVector
                                     , int32 iMaxRecurseDepth )
{
    FVec2D point0 = iSegment.GetPoint(0).GetCoords();
    FVec2D point1 = iSegment.GetPoint(1).GetCoords();
    FVec2D ctrlPoint0 = iSegment.GetControlPoint(0).GetCoords();
    FVec2D ctrlPoint1 = iSegment.GetControlPoint(1).GetCoords();
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
    static FVec2D nilVector = { 0.0f, 0.0F };
// DEBUG
    uint32 debugColor[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
// END DEBUG

    if( ( *iPrevSegmentVector != nilVector ) && iPrevSegmentVector->DotProduct( subTangent[0] ) <= angleCosineLimit )
    {
        doRefine = true;
    }

    if( ( *iNextSegmentVector != nilVector ) && iNextSegmentVector->DotProduct( subTangent[2] ) <= angleCosineLimit )
    {
        doRefine = true;
    }

    /*straightVector.Normalize();
    straightVector.Normalize();*/
   /* subTangent[0].Normalize();
    subSegment[0].Normalize();*/

    /*if( subTangent[0].DotProduct( subSegment[0] ) <= angleCosineLimit )
    {
        doRefine = true;
    }*/

    /*if( subTangent[2].DotProduct( straightVector ) <= angleCosineLimit )
    {
        doRefine = true;
    }*/

    for( int i = 0; i < 3; i++ )
    {
        int n = ( i + 1 );
        double subTanDistanceSquared = subTangent[i].DistanceSquared();
        double subSegDistanceSquared = subSegment[i].DistanceSquared();

        if( subTanDistanceSquared && subSegDistanceSquared )
        {
            /*if( ULIS::FMath::EpsilonComp( subTanDistanceSquared, 1.0f ) == false )
            {*/
                subTangent[i].Normalize();
                subSegment[i].Normalize();

                if(subTangent[i].DotProduct(subSegment[i]) <= angleCosineLimit)
                {
                    doRefine = true;
                }
            /*}*/
        }
    }

    // TODO : case when the line is straight or almost straight
    /*if( subTangent[0].DotProduct( subTangent[2] ) >= angleCosineLimit )
    {
        doRefine = true;
    }*/

    /*for( int i = 0; i < 2; i++ )
    {
        int n = ( i + 1 );

        if( subTangent[i].DotProduct( subSegment[i] ) <= angleCosineLimit )
        {
            doRefine = true;
        }
    }*/

    for( int i = 0; i < 3; i++ )
    {
        int n = ( i + 1 );

        nextSegmentVector = ( i == 0x02 ) ? iNextSegmentVector : &subTangent[n];
/*
        if( prevSegmentVector->x == 0.0f && prevSegmentVector->y == 0.0f ) doRefine = true;
        if( nextSegmentVector->x == 0.0f && nextSegmentVector->y == 0.0f ) doRefine = true;
*/
        if( ( doRefine == true ) && --iMaxRecurseDepth >= 0 )
        {

            DrawSegmentVariable( iBlock
                               , iBLContext
                               , iSegment
                               , sampleRange[i]
                               , sampleRange[n]
                               , radius
                               , radius + radiusStep
                               , prevSegmentVector
                               , nextSegmentVector
                               , iMaxRecurseDepth );
        }
        else
        {
            DrawSegmentVariableThickness( iBlock
                                        , iBLContext
                                        , iSegment
                                        , sampleRange[i]
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
FVectorPathCubic::DrawJoint( FBlock& iBlock
                           , BLContext& iBLContext
                           , FVectorSegmentCubic& iPrevSegment
                           , FVectorSegmentCubic& iSegment
                           , double iRadius
                           , FVec2D* iPrevSegmentVector )
{
    FVec2D& point0 = iSegment.GetPoint(0).GetCoords();
    FVec2D& point1 = iSegment.GetPoint(0).GetCoords();
    FVec2D& ctrlPoint0 = iSegment.GetControlPoint(0).GetCoords();
    FVec2D vec = { ctrlPoint0 - point0 };

    vec.Normalize();

    FVec2D currPerpendicularVec = { vec.y, - vec.x };
    FVec2D prevPerpendicularVec = { iPrevSegmentVector->y, - iPrevSegmentVector->x };
    FVec2D edgePrevPoint = point0 + ( prevPerpendicularVec * iRadius );
    FVec2D edgePoint = point0 + ( currPerpendicularVec * iRadius );
    FVec2D testvec = edgePoint - edgePrevPoint;
    double angle = acos(currPerpendicularVec.DotProduct(prevPerpendicularVec));
    double a = angle / 12;

    if ( testvec.DotProduct( *iPrevSegmentVector ) < 0 )
    {
        FVec2D tmp = currPerpendicularVec;
a = -a;
        currPerpendicularVec = -tmp;

               tmp = prevPerpendicularVec;
        prevPerpendicularVec = -tmp;
    }

    BLPoint vertex[3];

    vertex[0].x = (point0.x);
    vertex[0].y = (point0.y);

    for ( int i = 0; i < 12; i++ )
    {
        FVec2D interpolatedVector = { currPerpendicularVec.x * cos(a) + currPerpendicularVec.y * sin(a),
                                     -currPerpendicularVec.x * sin(a) + currPerpendicularVec.y * cos(a) };

        vertex[1].x = vertex[0].x + (currPerpendicularVec.x * iRadius);
        vertex[1].y = vertex[0].y + (currPerpendicularVec.y * iRadius);

        vertex[2].x = vertex[0].x + (interpolatedVector.x * iRadius);
        vertex[2].y = vertex[0].y + (interpolatedVector.y * iRadius);

        iBLContext.strokePolygon( vertex , 3 );  

        currPerpendicularVec = interpolatedVector;
    }    
}

void
FVectorPathCubic::DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext )
{
    /*double radiusDiff = iEndRadius - iStartRadius;
    double radiusStep = (mSegmentList.size() == 0) ? 0 : radiusDiff / mSegmentList.size();*/
    double mStartRadius = 10.0f;
    double mEndRadius = 0.0f;

    iBLContext.setCompOp( BL_COMP_OP_SRC_COPY );

    // We fill with stroke color because our curve is made of filled shapes.
    iBLContext.setFillStyle(BLRgba32(mStrokeColor));
    iBLContext.setStrokeStyle(BLRgba32(mStrokeColor));

    if( mSegmentList.size() )
    {
        double stepRadius = ( mEndRadius - mStartRadius ) / mSegmentList.size();
        double segmentStartRadius = mStartRadius;
        FVectorSegmentCubic* prevSegment = nullptr;

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
            double segmentStartRadius = segment->GetPoint(0).GetRadius();
            double segmentEndRadius = segment->GetPoint(1).GetRadius();
            FVec2D point0 = segment->GetPoint(0).GetCoords();
            FVec2D prevSegmentVector = segment->GetPreviousVector();
            FVec2D nextSegmentVector = segment->GetNextVector();
            static FVec2D zeroVector = { 0.0f, 0.0f };
/*
printf("prev: %f %f\n", prevSegmentVector.x, prevSegmentVector.y );
printf("next: %f %f\n", nextSegmentVector.x, nextSegmentVector.y );
*/
            DrawSegmentVariable ( iBlock
                                , iBLContext
                                , *segment
                                , 0.0f
                                , 1.0f
                                , segmentStartRadius
                                , segmentEndRadius
                                , &zeroVector/*prevSegmentVector*/
                                , &zeroVector/*nextSegmentVector*/
                                , 8 );

            if ( prevSegment )
            {
                DrawJoint( iBlock
                         , iBLContext
                         , *prevSegment
                         , *segment
                         , segmentStartRadius
                         , &prevSegmentVector );
            }

            /*iBLContext.strokeCircle( point0.x, point0.y, segmentStartRadius );*/
            /*iBLContext.fillCircle( point0.x, point0.y, segmentStartRadius );*/

            segmentStartRadius = segmentEndRadius;

            prevSegment = segment;
        }
    }
}

#ifdef unused
void
FVectorPathCubic::DrawSegmentVariable( FBlock& iBlock
                                     , BLContext& iBLContext
                                     , FVectorSegmentCubic& iSegment
                                     , double iStartRadius
                                     , double iEndRadius )
{
    BLPoint point0;
    BLPoint point1;
    BLPoint ctrlPoint0;
    BLPoint ctrlPoint1;
    FVec2D  samplePoint0;
    FVec2D  samplePoint1;
    FVec2D  perpendicularVec0;
    FVec2D  perpendicularVec1;
    FVec2D  parallelVec0;
    FVec2D  parallelVec1;
    double perpendicularVec0DistanceSquared;
    double perpendicularVec1DistanceSquared;

    point0.x = iSegment.GetPoint(0).GetX();
    point0.y = iSegment.GetPoint(0).GetY();

    ctrlPoint0.x = iSegment.GetControlPoint(0).GetX();
    ctrlPoint0.y = iSegment.GetControlPoint(0).GetY();

    ctrlPoint1.x = iSegment.GetControlPoint(1).GetX();
    ctrlPoint1.y = iSegment.GetControlPoint(1).GetY();

    point1.x = iSegment.GetPoint(1).GetX();
    point1.y = iSegment.GetPoint(1).GetY();

    samplePoint0 = CubicBezierPointAtParameter<FVec2D>( iSegment.GetPoint(0).GetCoords()
                                                      , iSegment.GetControlPoint(0).GetCoords()
                                                      , iSegment.GetControlPoint(1).GetCoords()
                                                      , iSegment.GetPoint(1).GetCoords()
                                                      , 0.01f );

    parallelVec0 = samplePoint0 - iSegment.GetPoint(0).GetCoords();
    perpendicularVec0.x =   parallelVec0.y;
    perpendicularVec0.y = - parallelVec0.x;

    samplePoint1 = CubicBezierPointAtParameter<FVec2D>( iSegment.GetPoint(0).GetCoords()
                                                      , iSegment.GetControlPoint(0).GetCoords()
                                                      , iSegment.GetControlPoint(1).GetCoords()
                                                      , iSegment.GetPoint(1).GetCoords()
                                                      , 0.99f );

    parallelVec1 = samplePoint1 - iSegment.GetPoint(1).GetCoords();
    perpendicularVec1.x =   parallelVec1.y;
    perpendicularVec1.y = - parallelVec1.x;

    perpendicularVec0DistanceSquared = perpendicularVec0.DistanceSquared();
    perpendicularVec1DistanceSquared = perpendicularVec1.DistanceSquared();

    if( perpendicularVec0DistanceSquared && perpendicularVec1DistanceSquared )
    {
        FVec2D perpendicularVec0Right;
        FVec2D perpendicularVec0Left;
        FVec2D perpendicularVec1Right;
        FVec2D perpendicularVec1Left;
        BLPoint perpendicularPoint0Right;
        BLPoint perpendicularPoint0Left;
        BLPoint perpendicularPoint1Right;
        BLPoint perpendicularPoint1Left;
        BLPath pathRight;
        BLPath pathLeft;
        double correctionFactor0Right = 1.0f;
        double correctionFactor0Left  = 1.0f;
        double correctionFactor1Right = 1.0f;
        double correctionFactor1Left  = 1.0f;

        // TODO: this can be improved, knowing that our sample points are taken at values 0.01f and 0.99f, it should not be to hard to normalize without sqrts
        perpendicularVec0.Normalize();
        perpendicularVec1.Normalize();

        perpendicularVec0Right =   ( perpendicularVec0 * iStartRadius );
        perpendicularVec0Left  = - ( perpendicularVec0Right );

        perpendicularVec1Right =   ( perpendicularVec1 * iEndRadius   );
        perpendicularVec1Left  = - ( perpendicularVec1Right   );

        perpendicularPoint0Right.x = point0.x + perpendicularVec0Right.x;
        perpendicularPoint0Right.y = point0.y + perpendicularVec0Right.y;

        perpendicularPoint0Left.x  = point0.x + perpendicularVec0Left.x;
        perpendicularPoint0Left.y  = point0.y + perpendicularVec0Left.y;

        perpendicularPoint1Right.x = point1.x + perpendicularVec1Right.x;
        perpendicularPoint1Right.y = point1.y + perpendicularVec1Right.y;

        perpendicularPoint1Left.x  = point1.x + perpendicularVec1Left.x;
        perpendicularPoint1Left.y  = point1.y + perpendicularVec1Left.y;

        pathRight.moveTo( perpendicularPoint0Left  );
        pathRight.lineTo( perpendicularPoint0Right );

        // because pathLeft is going to be reversed (then concatenated), its first point must be "lineTo"
        /*path0.lineTo( perpendicularPoint0Right.x , perpendicularPoint0Right.y  );*/
        pathLeft.lineTo( perpendicularPoint0Left.x , perpendicularPoint0Left.y  );

        double segmentStraightDistance = iSegment.GetStraightDistance();

        // START - side control points length correction
        if( segmentStraightDistance )
        {
            FVec2D segmentStraightVector0 = { point1.x - point0.x
                                            , point1.y - point0.y };
            FVec2D segmentStraightVector1 = { point0.x - point1.x
                                            , point0.y - point1.y };
            FVec2D perpendicularPoint0RightToPoint1 = { point1.x - perpendicularPoint0Right.x
                                                      , point1.y - perpendicularPoint0Right.y };
            FVec2D perpendicularPoint0LeftToPoint1  = { point1.x - perpendicularPoint0Left.x
                                                      , point1.y - perpendicularPoint0Left.y };
            FVec2D perpendicularPoint1RightToPoint0 = { point0.x - perpendicularPoint1Right.x
                                                      , point0.y - perpendicularPoint1Right.y };
            FVec2D perpendicularPoint1LeftToPoint0  = { point0.x - perpendicularPoint1Left.x
                                                      , point0.y - perpendicularPoint1Left.y };
            FVec2D ctrlVector0 = { ctrlPoint0.x - point0.x
                                 , ctrlPoint0.y - point0.y };

            FVec2D ctrlVector1 = { ctrlPoint1.x - point1.x
                                 , ctrlPoint1.y - point1.y };

            segmentStraightVector0.Normalize();
            segmentStraightVector1.Normalize();

    // TODO: remove "* 1", this is just to visually increase difference for better debug
            double ratio0 = ( iStartRadius  * 2  ) / segmentStraightDistance;
            double ratio1 = ( iEndRadius    * 2  ) / segmentStraightDistance;
            double ctrl0Angle = /*fabs( */perpendicularVec0.DotProduct( segmentStraightVector0 );/* );*/
            double ctrl1Angle = /*fabs( */perpendicularVec1.DotProduct( segmentStraightVector1 );/* );*/

            correctionFactor0Right -= ( ratio0 * ctrl0Angle/* *   sign0*/ );
            correctionFactor1Left  += ( ratio1 * ctrl1Angle/* * - sign1*/ );

            correctionFactor0Left  += ( ratio0 * ctrl0Angle/* * - sign0*/ );
            correctionFactor1Right -= ( ratio1 * ctrl1Angle/* *   sign1*/ );
        }
        // END - side control points length correction

        DrawSegment( pathRight
                   , iSegment
                   , &perpendicularVec0Right
                   , &perpendicularVec1Left
                   , correctionFactor0Right
                   , correctionFactor1Left
                   , false );

        point0RightTmp = point0Tmp;
        ctrlPoint0RightTmp = ctrlPoint0Tmp;
        point1RightTmp = point1Tmp;
        ctrlPoint1RightTmp = ctrlPoint1Tmp;

        DrawSegment( pathLeft
                   , iSegment
                   , &perpendicularVec0Left
                   , &perpendicularVec1Right
                   , correctionFactor0Left
                   , correctionFactor1Right
                   , false );

        point0LeftTmp = point0Tmp;
        ctrlPoint0LeftTmp = ctrlPoint0Tmp;
        point1LeftTmp = point1Tmp;
        ctrlPoint1LeftTmp = ctrlPoint1Tmp;

        /*perpendicularPoint1Left.x  = point1.x + perpendicularVec1Left.x;
        perpendicularPoint1Left.y  = point1.y + perpendicularVec1Left.y;*/

        pathRight.lineTo( perpendicularPoint1Right );
        pathRight.addReversedPath( pathLeft, BL_PATH_REVERSE_MODE_COMPLETE );

        iBLContext.fillPath( pathRight );
    }
}

#endif

#ifdef unused
void
FVectorPathCubic::DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext )
{
    double mStartRadius = 10.0f;
    double mEndRadius = 0.0f;

    iBLContext.setCompOp( BL_COMP_OP_SRC_COPY );

    // We fill with stroke color because our curve is made of filled shapes.
    iBLContext.setFillStyle(BLRgba32(mStrokeColor));

    if( mSegmentList.size() )
    {
        double stepRadius = ( mEndRadius - mStartRadius ) / mSegmentList.size();
        double segmentStartRadius = mStartRadius;
        double segmentEndRadius = segmentStartRadius + stepRadius;

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            BLPoint point0 = { segment->GetPoint(0).GetX()
                             , segment->GetPoint(0).GetY() };

            DrawSegmentVariable ( iBlock, iBLContext, *segment, segmentStartRadius, segmentEndRadius );

            iBLContext.fillCircle( point0.x, point0.y, segmentStartRadius );

            iBLContext.setStrokeStyle(BLRgba32(0xFFFF00FF));
            iBLContext.setStrokeWidth(1.0f);
            BLPath tmpRight0;
            tmpRight0.moveTo( point0RightTmp.x, point0RightTmp.y );
            tmpRight0.lineTo( ctrlPoint0RightTmp.x, ctrlPoint0RightTmp.y );
            iBLContext.strokePath( tmpRight0 );

            iBLContext.setStrokeStyle(BLRgba32(0xFFFF00FF));
            iBLContext.setStrokeWidth(1.0f);
            BLPath tmpRight1;
            tmpRight1.moveTo(point1RightTmp.x,point1RightTmp.y);
            tmpRight1.lineTo(ctrlPoint1RightTmp.x,ctrlPoint1RightTmp.y);
            iBLContext.strokePath(tmpRight1);

            iBLContext.setStrokeStyle(BLRgba32(0xFFFF00FF));
            iBLContext.setStrokeWidth(1.0f);
            BLPath tmpLeft0;
            tmpLeft0.moveTo(point0LeftTmp.x,point0LeftTmp.y);
            tmpLeft0.lineTo(ctrlPoint0LeftTmp.x,ctrlPoint0LeftTmp.y);
            iBLContext.strokePath(tmpLeft0);

            iBLContext.setStrokeStyle(BLRgba32(0xFFFF00FF));
            iBLContext.setStrokeWidth(1.0f);
            BLPath tmpLeft1;
            tmpLeft1.moveTo(point1LeftTmp.x,point1LeftTmp.y);
            tmpLeft1.lineTo(ctrlPoint1LeftTmp.x,ctrlPoint1LeftTmp.y);
            iBLContext.strokePath(tmpLeft1);

            segmentStartRadius = segmentEndRadius;
            segmentEndRadius  += stepRadius;
        }
    }
}

#endif