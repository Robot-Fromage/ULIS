#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathCubic::FVectorPathCubic()
    : FVectorPath()
{
    setJointRadial();
}

void
FVectorPathCubic::setJointRadial()
{
    mJointType = JOINT_TYPE_RADIAL;
}

void
FVectorPathCubic::setJointLinear()
{
    mJointType = JOINT_TYPE_LINEAR;
}

void
FVectorPathCubic::setJointNone()
{
    mJointType = JOINT_TYPE_NONE;
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

        // Pick point handle
        if( ( fabs( point->GetX() + ( perpendicularVector.x * point->GetRadius() ) - iX ) <= iSelectionRadius ) &&
            ( fabs( point->GetY() + ( perpendicularVector.y * point->GetRadius() ) - iY ) <= iSelectionRadius ) )
        {
            mSelectedPointList.push_back( &point->GetControlPoint() );
        }

        // Pick point handle on the other side
        if( ( fabs( point->GetX() - ( perpendicularVector.x * point->GetRadius() ) - iX ) <= iSelectionRadius ) &&
            ( fabs( point->GetY() - ( perpendicularVector.y * point->GetRadius() ) - iY ) <= iSelectionRadius ) )
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
        double ctrlX = ( perpendicular.x * pointRadius );
        double ctrlY = ( perpendicular.y * pointRadius );

        iBLContext.setFillStyle( BLRgba32( 0xFFFF00FF ) );
        iBLContext.fillRect( point->GetX() - 3, point->GetY() - 3, 6, 6 );

        iBLContext.setFillStyle( BLRgba32( 0xFF808080 ) );
        iBLContext.fillRect( point->GetX() + ctrlX - 3, point->GetY() + ctrlY - 3, 6, 6 );
        iBLContext.fillRect( point->GetX() - ctrlX - 3, point->GetY() - ctrlY - 3, 6, 6 );
    }
}

void
FVectorPathCubic::DrawSegment( BLPath& iPath
                             , FVectorSegmentCubic& iSegment
                             , bool iIsStandalone )
{
    FVec2D& p0 = iSegment.GetPoint(0).GetCoords();
    FVec2D& p1 = iSegment.GetPoint(1).GetCoords();
    FVec2D& c0 = iSegment.GetControlPoint(0).GetCoords();
    FVec2D& c1 = iSegment.GetControlPoint(1).GetCoords();
    BLPoint point0 = { p0.x, p0.y };
    BLPoint point1 = { p1.x, p1.y };
    BLPoint ctrlPoint0 = { c0.x, c0.y };
    BLPoint ctrlPoint1 = { c1.x, c1.y };

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
    FVec2D sampleTangent[2] = { CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iFromT ),
                                CubicBezierTangentAtParameter<FVec2D>( point0, ctrlPoint0, ctrlPoint1, point1, iToT   ) };
    // Note: no need to normalize sampleTangent[] vectors. The averaging won't be any different than with a normalized version.
    FVec2D parallelVecFrom[2] = { *iPrevSegmentVector, sampleTangent[0]    };
    FVec2D   parallelVecTo[2] = { sampleTangent[1]   , *iNextSegmentVector };
    FVec2D averageVecFrom = ( parallelVecFrom[0] + parallelVecFrom[1] ) * 0.5f;
    FVec2D averageVecTo   = (   parallelVecTo[0] +   parallelVecTo[1] ) * 0.5f;
    FVec2D perpendicularVecFrom = { averageVecFrom.y, -averageVecFrom.x };
    FVec2D perpendicularVecTo = { averageVecTo.y, -averageVecTo.x };
    BLPoint vertex[4];

    if ( perpendicularVecFrom.DistanceSquared() && perpendicularVecTo.DistanceSquared() )
    {
        perpendicularVecFrom.Normalize();
        perpendicularVecTo.Normalize();
    }

    perpendicularVecFrom *= iStartRadius;
    perpendicularVecTo *= iEndRadius;

    vertex[0].x = iFromPoint.x + perpendicularVecFrom.x;
    vertex[0].y = iFromPoint.y + perpendicularVecFrom.y;

    vertex[1].x = iToPoint.x + perpendicularVecTo.x;
    vertex[1].y = iToPoint.y + perpendicularVecTo.y;

    vertex[2].x = iToPoint.x - perpendicularVecTo.x;
    vertex[2].y = iToPoint.y - perpendicularVecTo.y;

    vertex[3].x = iFromPoint.x - perpendicularVecFrom.x;
    vertex[3].y = iFromPoint.y - perpendicularVecFrom.y;

    iBLContext.strokePolygon( vertex, 4 );
    iBLContext.fillPolygon( vertex, 4 );
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
    double angleCosineLimit = 0.998f;
    static FVec2D nilVector = { 0.0f, 0.0f };
// DEBUG
    uint32 debugColor[3] = { 0xFFFF0000, 0xFF00FF00, 0xFF0000FF };
// END DEBUG

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

static void
_drawRadialJoint( FBlock& iBlock
                , BLContext& iBLContext
                , FVec2D& iOrigin
                , FVec2D& iPrevSegmentVector
                , FVec2D& iSegmentVector
                , double iRadius
                , uint32 iSteps )
{
    FVec2D currPerpendicularVec = { iSegmentVector.y    , - iSegmentVector.x     };
    FVec2D prevPerpendicularVec = { iPrevSegmentVector.y, - iPrevSegmentVector.x };
    FVec2D edgePrevPoint = iOrigin + ( prevPerpendicularVec * iRadius );
    FVec2D edgePoint = iOrigin + ( currPerpendicularVec * iRadius );
    FVec2D shortestTest = edgePoint - edgePrevPoint;
    // have to clamp due to imprecision of the dot product
    double dot = std::clamp<double>( currPerpendicularVec.DotProduct( prevPerpendicularVec ), -1.0f, 1.0f );
    double angle = acos( dot );
    double a = ( iSteps ) ? angle / iSteps : 0.0f;
    BLPoint vertex[3];

    // Find on which side should the joint be drawn by comparing the directions of our vectors
    if ( shortestTest.DotProduct( iPrevSegmentVector ) < 0 )
    {
        FVec2D tmp = currPerpendicularVec;
        currPerpendicularVec = -tmp;

               tmp = prevPerpendicularVec;
        prevPerpendicularVec = -tmp;

        a = -a;
    }

    // start drawing triangles at origin
    vertex[0].x = ( iOrigin.x );
    vertex[0].y = ( iOrigin.y );

    for ( int i = 0; i < iSteps; i++ )
    {
        double cosa = cos(a);
        double sina = sin(a);
        FVec2D interpolatedVector = { currPerpendicularVec.x * cosa + currPerpendicularVec.y * sina,
                                     -currPerpendicularVec.x * sina + currPerpendicularVec.y * cosa };

        vertex[1].x = vertex[0].x + ( currPerpendicularVec.x * iRadius );
        vertex[1].y = vertex[0].y + ( currPerpendicularVec.y * iRadius );

        vertex[2].x = vertex[0].x + ( interpolatedVector.x * iRadius );
        vertex[2].y = vertex[0].y + ( interpolatedVector.y * iRadius );

        iBLContext.strokePolygon( vertex , 3 );
        iBLContext.fillPolygon( vertex, 3 );

        currPerpendicularVec = interpolatedVector;
    }
}

static void
_drawLinearJoint( FBlock& iBlock
                , BLContext& iBLContext
                , FVec2D& iOrigin
                , FVec2D& iPrevSegmentVector
                , FVec2D& iSegmentVector
                , double iRadius )
{
    FVec2D currPerpendicularVec = { iSegmentVector.y    , - iSegmentVector.x     };
    FVec2D prevPerpendicularVec = { iPrevSegmentVector.y, - iPrevSegmentVector.x };
    FVec2D edgePrevPoint = iOrigin + ( prevPerpendicularVec * iRadius );
    FVec2D edgePoint = iOrigin + ( currPerpendicularVec * iRadius );
    FVec2D shortestTest = edgePoint - edgePrevPoint;

    BLPoint vertex[3];

    if ( shortestTest.DotProduct( iPrevSegmentVector ) < 0 )
    {
        FVec2D tmp = currPerpendicularVec;

        currPerpendicularVec = -tmp;

                tmp = prevPerpendicularVec;
        prevPerpendicularVec = -tmp;
    }

    vertex[0].x = ( iOrigin.x );
    vertex[0].y = ( iOrigin.y );

    vertex[1].x = vertex[0].x + ( currPerpendicularVec.x * iRadius );
    vertex[1].y = vertex[0].y + ( currPerpendicularVec.y * iRadius );

    vertex[2].x = vertex[0].x + ( prevPerpendicularVec.x * iRadius );
    vertex[2].y = vertex[0].y + ( prevPerpendicularVec.y * iRadius );

    iBLContext.strokePolygon( vertex, 3 );
    iBLContext.fillPolygon( vertex, 3 );
}

void
FVectorPathCubic::DrawJoint( FBlock& iBlock
                           , BLContext& iBLContext
                           , FVectorSegmentCubic* iPrevSegment
                           , FVectorSegmentCubic& iSegment
                           , double iRadius )
{
    if ( iPrevSegment )
    {
        FVec2D prevSegmentVector = iPrevSegment->GetVectorAtEnd( false );
        FVec2D segmentVector = iSegment.GetVectorAtStart( false );
        FVec2D& origin = iSegment.GetPoint(0).GetCoords();

        if ( prevSegmentVector.DistanceSquared() && segmentVector.DistanceSquared() )
        {
            prevSegmentVector.Normalize();
            segmentVector.Normalize();

            // if the dot product equals to 1.0f, then the point is perfectly smooth, hence there is no need for joints.
            if ( prevSegmentVector.DotProduct(segmentVector) != 1.0f )
            {
                switch ( mJointType )
                {
                    case JOINT_TYPE_LINEAR :
                        _drawLinearJoint ( iBlock, iBLContext, origin, prevSegmentVector, segmentVector, iRadius );
                    break;

                    case JOINT_TYPE_RADIAL :
                        _drawRadialJoint ( iBlock, iBLContext, origin, prevSegmentVector, segmentVector, iRadius, 24 );
                    break;

                    default:
                    break;
                }
            }
        }
    }
}

void
FVectorPathCubic::DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext )
{
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
            static FVec2D zeroVector = { 0.0f, 0.0f };

            DrawSegmentVariable ( iBlock
                                , iBLContext
                                , *segment
                                , 0.0f
                                , 1.0f
                                , segmentStartRadius
                                , segmentEndRadius
                                , &zeroVector
                                , &zeroVector
                                , 8 );

            if ( prevSegment )
            {
                DrawJoint( iBlock
                         , iBLContext
                         , prevSegment
                         , *segment
                         , segmentStartRadius );
            }

            segmentStartRadius = segmentEndRadius;

            prevSegment = segment;
        }
    }
}
