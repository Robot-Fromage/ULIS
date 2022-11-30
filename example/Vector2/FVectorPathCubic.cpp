#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathCubic::FVectorPathCubic()
    : FVectorPath()
{
    setJointMiter();
}

FVectorPathCubic::FVectorPathCubic( std::string iName )
    : FVectorPath( iName )
{

}

void
FVectorPathCubic::setJointRadial()
{
    mJointType = JOINT_TYPE_RADIAL;
}

void
FVectorPathCubic::setJointMiter()
{
    mJointType = JOINT_TYPE_MITER;
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
FVectorPathCubic::AppendPoint( FVectorPointCubic* iPoint
                             , bool iConnect
                             , bool iBuildSegments )
{
    FVectorPointCubic* lastPoint = static_cast<FVectorPointCubic*>( GetLastPoint() );

    mPointList.push_back ( iPoint );

    if ( iConnect == true )
    {
        // lastPoint is NULL if this is the first point added
        if( lastPoint )
        {
            FVectorSegmentCubic* segment = new FVectorSegmentCubic( *this, lastPoint, iPoint );

            AddSegment( segment );

            if( iBuildSegments == true )
            {
                iPoint->BuildSegments();
            }

            return segment;
        }
    }

    return nullptr;
}

FVectorObject*
FVectorPathCubic::PickShape( double iX
                           , double iY
                           , double iRadius )
{
    BLPath path;
    BLPoint testPoint = { iX, iY };
    uint32 hitResult;
/*
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVec2D& point0 = segment->GetPoint(0).GetCoords();
        FVec2D& point1 = segment->GetPoint(1).GetCoords();
        FVec2D& ctrlPoint0 = segment->GetControlPoint(0).GetCoords();
        FVec2D& ctrlPoint1 = segment->GetControlPoint(1).GetCoords();

        path.moveTo( point0.x, point0.y );
        path.cubicTo( ctrlPoint0.x
                    , ctrlPoint0.y
                    , ctrlPoint1.x
                    , ctrlPoint1.y
                    , point1.x
                    , point1.y );
    }

    if ( path.hitTest( testPoint, BL_FILL_RULE_NON_ZERO ) == BL_HIT_TEST_IN )
    {
        return true;
    }
*/
    // Pick inside the polygons that makes the segment
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(*it);

        if ( cubicSegment->Pick ( iX, iY, iRadius ) == true )
        {
            return this;
        }
    }

    return PickLoops( iX, iY, iRadius );
}

bool
FVectorPathCubic::PickPoint( double iX
                           , double iY
                           , double iSelectionRadius
                           , uint64 iSelectionFlags )
{
    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {
        FVectorPointCubic* point = static_cast<FVectorPointCubic*>(*it);
        FVec2D perpendicularVector = point->GetPerpendicularVector( true );

        if ( iSelectionFlags & PICK_POINT )
        {
            if( ( fabs( point->GetX() - iX ) <= iSelectionRadius ) &&
                ( fabs( point->GetY() - iY ) <= iSelectionRadius ) )
            {
                mSelectedPointList.push_back( point );
            }
        }

        if( iSelectionFlags & PICK_HANDLE_POINT )
        {
            // Pick point handle
            if( ( fabs( point->GetX() + ( perpendicularVector.x * point->GetRadius() ) - iX ) <= iSelectionRadius ) &&
                ( fabs( point->GetY() + ( perpendicularVector.y * point->GetRadius() ) - iY ) <= iSelectionRadius ) )
            {
                mSelectedPointList.push_back( &point->GetControlPoint() );

                return true;
            }

            // Pick point handle on the other side
            if( ( fabs( point->GetX() - ( perpendicularVector.x * point->GetRadius() ) - iX ) <= iSelectionRadius ) &&
                ( fabs( point->GetY() - ( perpendicularVector.y * point->GetRadius() ) - iY ) <= iSelectionRadius ) )
            {
                mSelectedPointList.push_back( &point->GetControlPoint() );

                return true;
            }
        }
    }

    if( iSelectionFlags & PICK_HANDLE_SEGMENT )
    {
        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
            FVectorPoint& ctrlPoint0 = segment->GetControlPoint( 0 );
            FVectorPoint& ctrlPoint1 = segment->GetControlPoint( 1 );

            if( ( fabs( ctrlPoint0.GetX() - iX ) <= iSelectionRadius ) &&
                ( fabs( ctrlPoint0.GetY() - iY ) <= iSelectionRadius ) )
            {
                mSelectedPointList.push_back( &ctrlPoint0 );

                return true;
            }

            if( ( fabs( ctrlPoint1.GetX() - iX ) <= iSelectionRadius ) &&
                ( fabs( ctrlPoint1.GetY() - iY ) <= iSelectionRadius ) )
            {
                mSelectedPointList.push_back( &ctrlPoint1 );

                return true;
            }
        }
    }

    return false;
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
FVectorPathCubic::Cut( FVec2D& linePoint0
                     , FVec2D& linePoint1 )
{
    // let's work on a copy as we are going to remove items in the original list
    std::list<FVectorSegment*> tmpSegmentList = mSegmentList;

    while( tmpSegmentList.size () )
    {
        FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(tmpSegmentList.back());

        if ( cubicSegment->Cut ( linePoint0, linePoint1 ) )
        {
            /*mSegmentList.remove ( cubicSegment );*/ // commented out: this is in the cut func

            cubicSegment->Invalidate();
        }

        tmpSegmentList.pop_back ();
    }
}

void
FVectorPathCubic::Fill( FRectD& iRoi )
{
    FVectorPointCubic *firstPoint = static_cast<FVectorPointCubic*>( GetFirstPoint() );
    BLContext& blctx = FVectorEngine::GetBLContext();
    BLPath path;

    if ( IsLoop() && firstPoint )
    {
        blctx.setCompOp( BL_COMP_OP_SRC_COPY );
        /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
        iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

        path.moveTo( firstPoint->GetX(), firstPoint->GetY() );

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            FVec2D& point0 = segment->GetPoint(0)->GetCoords();
            FVec2D& point1 = segment->GetPoint(1)->GetCoords();
            FVec2D& ctrlPoint0 = segment->GetControlPoint(0).GetCoords();
            FVec2D& ctrlPoint1 = segment->GetControlPoint(1).GetCoords();

            path.cubicTo( ctrlPoint0.x
                        , ctrlPoint0.y
                        , ctrlPoint1.x
                        , ctrlPoint1.y
                        , point1.x
                        , point1.y );
        }

        blctx.setFillStyle( BLRgba32( mFillColor ) );
        blctx.fillPath( path );
    }
}

void
FVectorPathCubic::DrawStructure( FRectD& iRoi )
{
    BLContext& blctx = FVectorEngine::GetBLContext();
    BLPath path;
    FVectorPointCubic *firstPoint = static_cast<FVectorPointCubic*>( GetFirstPoint() );
    BLPoint localVector = mInverseWorldMatrix.mapVector ( 1.0f, 0.0f );
    FVec2D ulisVector = { localVector.x, localVector.y };
    double zoomFactor = ulisVector.Distance();
    double handleSize = 6 * zoomFactor;
    double handleHalfSize = handleSize * 0.5f;

    if ( firstPoint )
    {
        blctx.setCompOp( BL_COMP_OP_SRC_COPY );
        /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
        iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            FVec2D& point0 = segment->GetPoint( 0 )->GetCoords();
            FVec2D& point1 = segment->GetPoint( 1 )->GetCoords();
            FVec2D& ctrlPoint0 = segment->GetControlPoint( 0 ).GetCoords();
            FVec2D& ctrlPoint1 = segment->GetControlPoint( 1 ).GetCoords();
/*
            printf("%f %f - %f %f - %f %f - %f %f\n", point0.x
                                                    , point0.y
                                                    , ctrlPoint0.x
                                                    , ctrlPoint0.y
                                                    , ctrlPoint1.x
                                                    , ctrlPoint1.y
                                                    , point1.x
                                                    , point1.y );
*/
/*
            path.moveTo( point0.x, point0.y );
            path.cubicTo( ctrlPoint0.x
                        , ctrlPoint0.y
                        , ctrlPoint1.x
                        , ctrlPoint1.y
                        , point1.x
                        , point1.y );
*/
        }

        blctx.setStrokeStyle( BLRgba32( 0xFF00FF00 ) );
        blctx.setStrokeWidth( zoomFactor );
        blctx.strokePath( path );

        for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);

            segment->DrawStructure( iRoi, zoomFactor );
        }
    }

    // Points and Point size handles
    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {

        FVectorPointCubic *point = static_cast<FVectorPointCubic*>(*it);
        FVec2D perpendicular = point->GetPerpendicularVector( true );
        double pointRadius = point->GetRadius();
        double ctrlX = ( perpendicular.x * pointRadius );
        double ctrlY = ( perpendicular.y * pointRadius );

        blctx.setFillStyle( BLRgba32( 0xFFFF00FF ) );
        blctx.fillRect( point->GetX() - handleHalfSize
                      , point->GetY() - handleHalfSize
                      , handleSize
                      , handleSize );

        blctx.setFillStyle( BLRgba32( 0xFF808080 ) );
        blctx.fillRect( point->GetX() + ctrlX - handleHalfSize
                      , point->GetY() + ctrlY - handleHalfSize
                      , handleSize
                      , handleSize );

        blctx.fillRect( point->GetX() - ctrlX - handleHalfSize
                      , point->GetY() - ctrlY - handleHalfSize
                      , handleSize
                      , handleSize );
    }
}

void
FVectorPathCubic::DrawShape( FRectD &iRoi, uint64 iFlags )
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

    if ( IsFilled() )
    {
        Fill( iRoi );
    }

    DrawLoops( iRoi, iFlags  );

    DrawShapeVariable( iRoi, iFlags );

    if( mIsSelected )
    {
        DrawStructure( iRoi );
    }
/*
        iBLContext.setStrokeWidth( 6 );
    DrawLoops( iRoi  );
*/
}

// https://gamedev.net/forums/topic/647810-intersection-point-of-two-vectors/5094071/
static bool intersectLine( FVec2D& iOrigin0
                         , FVec2D& iDirection0
                         , FVec2D& iOrigin1
                         , FVec2D& iDirection1
                         , FVec2D& iOut ) {
    FVec2D c = iOrigin0 - iOrigin1;
    double cross = ( iDirection0.y * iDirection1.x ) - ( iDirection0.x * iDirection1.y );

    if ( cross )
    {
        double t = ( ( c.x * iDirection1.y ) - ( c.y * iDirection1.x ) ) / cross;

        iOut = iOrigin0 + ( iDirection0 * t );

        return true;
    }

    return false;
}

static void
_drawMiterJoint( FVec2D& iOrigin
               , FVec2D& iPrevSegmentVector
               , FVec2D& iSegmentVector
               , double iRadius
               , double iMiterLimit )
{
    FVec2D currParallelVec = - iSegmentVector;
    FVec2D prevParallelVec = iPrevSegmentVector;
    FVec2D currPerpendicularVec = { iSegmentVector.y    , - iSegmentVector.x     };
    FVec2D prevPerpendicularVec = { iPrevSegmentVector.y, - iPrevSegmentVector.x };
    FVec2D edgePrevPoint = iOrigin + ( prevPerpendicularVec * iRadius );
    FVec2D edgePoint = iOrigin + ( currPerpendicularVec * iRadius );
    FVec2D shortestTest = edgePoint - edgePrevPoint;
    // have to clamp due to imprecision of the dot product
    double dot = std::clamp<double>( currPerpendicularVec.DotProduct( prevPerpendicularVec ), -1.0f, 1.0f );
    BLContext& blctx = FVectorEngine::GetBLContext();

    FVec2D intersectionPoint;

    // Find on which side should the joint be drawn by comparing the directions of our vectors
    if ( shortestTest.DotProduct( iPrevSegmentVector ) < 0 )
    {
        FVec2D tmp = currPerpendicularVec;
        currPerpendicularVec = -tmp;

               tmp = prevPerpendicularVec;
        prevPerpendicularVec = -tmp;

        edgePrevPoint = iOrigin + ( prevPerpendicularVec * iRadius );
        edgePoint = iOrigin + ( currPerpendicularVec * iRadius );
    }

    if( iRadius )
    {
        if ( intersectLine( edgePoint
                          , currParallelVec
                          , edgePrevPoint
                          , prevParallelVec
                          , intersectionPoint ) == true )
        {
            FVec2D originToIntersection = intersectionPoint - iOrigin;

            double miterRatio = originToIntersection.Distance() / iRadius;

            if ( miterRatio < iMiterLimit )
            {
                BLPoint vertex[4];
                vertex[0].x = iOrigin.x;
                vertex[0].y = iOrigin.y;

                vertex[1].x = edgePoint.x;
                vertex[1].y = edgePoint.y;

                vertex[2].x = intersectionPoint.x;
                vertex[2].y = intersectionPoint.y;

                vertex[3].x = edgePrevPoint.x;
                vertex[3].y = edgePrevPoint.y;

                blctx.strokePolygon( vertex, 4 );
                blctx.fillPolygon( vertex, 4 );
            }
            else
            {
                BLPoint vertex[5];
                vertex[0].x = iOrigin.x;
                vertex[0].y = iOrigin.y;

                vertex[1].x = edgePoint.x;
                vertex[1].y = edgePoint.y;

                vertex[2].x = edgePoint.x + ( currParallelVec.x * iMiterLimit * iRadius );
                vertex[2].y = edgePoint.y + ( currParallelVec.y * iMiterLimit * iRadius );

                vertex[3].x = edgePrevPoint.x + ( prevParallelVec.x * iMiterLimit * iRadius );
                vertex[3].y = edgePrevPoint.y + ( prevParallelVec.y * iMiterLimit * iRadius );

                vertex[4].x = edgePrevPoint.x;
                vertex[4].y = edgePrevPoint.y;

                blctx.strokePolygon( vertex, 5 );
                blctx.fillPolygon( vertex, 5 );
            }
        }
    }
}

static void
_drawRadialJoint( FVec2D& iOrigin
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
    BLContext& blctx = FVectorEngine::GetBLContext();

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

        blctx.strokePolygon( vertex , 3 );
        blctx.fillPolygon( vertex, 3 );

        currPerpendicularVec = interpolatedVector;
    }
}

static void
_drawLinearJoint( FVec2D& iOrigin
                , FVec2D& iPrevSegmentVector
                , FVec2D& iSegmentVector
                , double iRadius )
{
    FVec2D currPerpendicularVec = { iSegmentVector.y    , - iSegmentVector.x     };
    FVec2D prevPerpendicularVec = { iPrevSegmentVector.y, - iPrevSegmentVector.x };
    FVec2D edgePrevPoint = iOrigin + ( prevPerpendicularVec * iRadius );
    FVec2D edgePoint = iOrigin + ( currPerpendicularVec * iRadius );
    FVec2D shortestTest = edgePoint - edgePrevPoint;
    BLContext& blctx = FVectorEngine::GetBLContext();
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

    blctx.strokePolygon( vertex, 3 );
    blctx.fillPolygon( vertex, 3 );
}

void
FVectorPathCubic::DrawJoint( FVectorSegmentCubic* iPrevSegment
                           , FVectorSegmentCubic& iSegment
                           , double iRadius )
{
    if ( iPrevSegment )
    {
        FVec2D prevSegmentVector = iPrevSegment->GetVectorAtEnd( false );
        FVec2D segmentVector = iSegment.GetVectorAtStart( false );
        FVec2D& origin = iSegment.GetPoint(0)->GetCoords();

        if ( prevSegmentVector.DistanceSquared() && segmentVector.DistanceSquared() )
        {
            prevSegmentVector.Normalize();
            segmentVector.Normalize();

            // if the dot product equals to 1.0f, then the point is perfectly smooth, hence there is no need for joints.
            if ( prevSegmentVector.DotProduct(segmentVector) < 1.0f )
            {
                switch ( mJointType )
                {
                    case JOINT_TYPE_LINEAR :
                        _drawLinearJoint ( origin, prevSegmentVector, segmentVector, iRadius );
                    break;

                    case JOINT_TYPE_MITER :
                        _drawMiterJoint ( origin, prevSegmentVector, segmentVector, iRadius, 4.0f );
                    break;

                    case JOINT_TYPE_RADIAL :
                        _drawRadialJoint ( origin, prevSegmentVector, segmentVector, iRadius, 24 );
                    break;

                    default:
                    break;
                }
            }
        }
    }
}

void
FVectorPathCubic::DrawShapeVariable( FRectD &iRoi, uint64 iFlags )
{
    BLContext& blctx = FVectorEngine::GetBLContext();
    blctx.setCompOp( BL_COMP_OP_SRC_OVER );

    // We fill with stroke color because our curve is made of filled shapes.
    blctx.setFillStyle(BLRgba32(mStrokeColor));
    blctx.setStrokeStyle(BLRgba32(mStrokeColor));

    if( mSegmentList.size() )
    {
        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
            FVectorSegmentCubic* prevSegment = static_cast<FVectorSegmentCubic*>( segment->GetPreviousSegment() );
            double segmentStartRadius = segment->GetPoint(0)->GetRadius();
            FRectD clip = iRoi & segment->GetBoundingBox();

            if( ( iRoi.Area() == 0.0f ) || clip.Area() )
            {
                segment->Draw( iRoi );
            }

            if ( prevSegment )
            {
                DrawJoint( prevSegment
                         , *segment
                         , segmentStartRadius );
            }
        }
    }
}

void
FVectorPathCubic::Mirror( bool iMirrorX, bool iMirrorY )
{
    double factorX = ( iMirrorX ) ? -1.0f : 1.0f;
    double factorY = ( iMirrorY ) ? -1.0f : 1.0f;

    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPointCubic* cubicPoint = static_cast<FVectorPointCubic*>(*it);

        cubicPoint->Set( cubicPoint->GetX() * factorX, cubicPoint->GetY() * factorY );
    }

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorHandleSegment& ctrlPoint0 = static_cast<FVectorHandleSegment&>(cubicSegment->GetControlPoint(0));
        FVectorHandleSegment& ctrlPoint1 = static_cast<FVectorHandleSegment&>(cubicSegment->GetControlPoint(1));

        ctrlPoint0.Set( ctrlPoint0.GetX() * factorX, ctrlPoint0.GetY() * factorY );
        ctrlPoint1.Set( ctrlPoint1.GetX() * factorX, ctrlPoint1.GetY() * factorY );
    }

    /*Update();*/
}

FVectorObject*
FVectorPathCubic::CopyShape()
{
    FVectorPathCubic* cubicPathCopy = new FVectorPathCubic ( );
    std::map<FVectorPointCubic*, FVectorPointCubic*> lookupTable;

    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPointCubic* originalPoint = static_cast<FVectorPointCubic*>(*it);
        FVectorPointCubic* newPoint = new FVectorPointCubic( originalPoint->GetX()
                                                           , originalPoint->GetY()
                                                           , originalPoint->GetRadius() );

        lookupTable.insert( std::make_pair( originalPoint, newPoint ) );

        cubicPathCopy->AddPoint( newPoint );
    }

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* originalSegment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPointCubic* point0 = static_cast<FVectorPointCubic*>( originalSegment->GetPoint(0) );
        FVectorPointCubic* point1 = static_cast<FVectorPointCubic*>( originalSegment->GetPoint(1) );
        FVectorSegmentCubic* newSegment = new FVectorSegmentCubic( *cubicPathCopy
                                                                 , lookupTable[point0]
                                                                 , originalSegment->GetControlPoint(0).GetX()
                                                                 , originalSegment->GetControlPoint(0).GetY()
                                                                 , originalSegment->GetControlPoint(1).GetX()
                                                                 , originalSegment->GetControlPoint(1).GetY()
                                                                 , lookupTable[point1] );

        cubicPathCopy->AddSegment( newSegment );

        newSegment->BuildVariable();
    }

    return static_cast<FVectorObject*>( cubicPathCopy );
}

void
FVectorPathCubic::Merge( FVectorPathCubic& iCubicPath )
{
    std::list<FVectorSegment*>& segmentList = iCubicPath.mSegmentList;
    std::list<FVectorPoint*>& pointList = iCubicPath.mPointList;

    std::map<FVectorPointCubic*, FVectorPointCubic*> lookupTable;

    for( std::list<FVectorPoint*>::iterator it = pointList.begin(); it != pointList.end(); ++it )
    {
        FVectorPointCubic* originalPoint = static_cast<FVectorPointCubic*>(*it);
        FVectorPointCubic* newPoint = new FVectorPointCubic( originalPoint->GetX()
                                                           , originalPoint->GetY()
                                                           , originalPoint->GetRadius() );

        lookupTable.insert( std::make_pair( originalPoint, newPoint ) );

        mPointList.push_back ( newPoint );
    }

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegmentCubic* originalSegment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPointCubic* point0 = static_cast<FVectorPointCubic*>( originalSegment->GetPoint(0) );
        FVectorPointCubic* point1 = static_cast<FVectorPointCubic*>( originalSegment->GetPoint(1) );
        FVectorSegmentCubic* newSegment = new FVectorSegmentCubic( *this
                                                                 , lookupTable[point0]
                                                                 , originalSegment->GetControlPoint(0).GetX()
                                                                 , originalSegment->GetControlPoint(0).GetY()
                                                                 , originalSegment->GetControlPoint(1).GetX()
                                                                 , originalSegment->GetControlPoint(1).GetY()
                                                                 , lookupTable[point1] );

        FVectorPath::AddSegment( newSegment );

        newSegment->BuildVariable();
    }
}
