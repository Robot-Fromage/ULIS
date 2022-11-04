#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathCubic::FVectorPathCubic()
    : FVectorPath()
{
    setJointRadial();
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

    FVectorPath::AppendPoint( iPoint );

    if ( iConnect == true )
    {
        // lastPoint is NULL if this is the first point added
        if( lastPoint )
        {
            FVectorSegmentCubic* segment = new FVectorSegmentCubic( lastPoint, iPoint );

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

bool
FVectorPathCubic::PickShape( BLContext& iBLContext
                           , double iX
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
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);

        if ( segment->Pick ( iX, iY, iRadius ) == true )
        {
            return true;
        }
    }

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
FVectorPathCubic::Fill( FBlock& iBlock
                      , BLContext& iBLContext
                      , FRectD& iRoi )
{
    BLPath path;
    FVectorPointCubic *firstPoint = static_cast<FVectorPointCubic*>( GetFirstPoint() );

    if ( IsLoop() && firstPoint )
    {

        iBLContext.setCompOp( BL_COMP_OP_SRC_COPY );
        /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
        iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

        path.moveTo( firstPoint->GetX(), firstPoint->GetY() );

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            FVec2D& point0 = segment->GetPoint(0).GetCoords();
            FVec2D& point1 = segment->GetPoint(1).GetCoords();
            FVec2D& ctrlPoint0 = segment->GetControlPoint(0).GetCoords();
            FVec2D& ctrlPoint1 = segment->GetControlPoint(1).GetCoords();

            path.cubicTo( ctrlPoint0.x
                        , ctrlPoint0.y
                        , ctrlPoint1.x
                        , ctrlPoint1.y
                        , point1.x
                        , point1.y );
        }

        iBLContext.setFillStyle( BLRgba32( mFillColor ) );
        iBLContext.fillPath( path );
    }
}

void
FVectorPathCubic::DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    BLPath path;
    FVectorPointCubic *firstPoint = static_cast<FVectorPointCubic*>( GetFirstPoint() );
    BLPoint localVector = mInverseWorldMatrix.mapVector ( 1.0f, 0.0f );
    FVec2D ulisVector = { localVector.x, localVector.y };
    double zoomFactor = ulisVector.Distance();
    double handleSize = 6 * zoomFactor;
    double handleHalfSize = handleSize * 0.5f;

    if ( firstPoint )
    {
        iBLContext.setCompOp( BL_COMP_OP_SRC_COPY );
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

            path.moveTo( point0.x, point0.y );
            path.cubicTo( ctrlPoint0.x
                        , ctrlPoint0.y
                        , ctrlPoint1.x
                        , ctrlPoint1.y
                        , point1.x
                        , point1.y );
        }

        iBLContext.setStrokeStyle( BLRgba32( 0xFF00FF00 ) );
        iBLContext.setStrokeWidth( zoomFactor );
        iBLContext.strokePath( path );

        for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);

            segment->DrawStructure( iBlock, iBLContext, iRoi, zoomFactor );
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


        iBLContext.setFillStyle( BLRgba32( 0xFFFF00FF ) );
        iBLContext.fillRect( point->GetX() - handleHalfSize
                           , point->GetY() - handleHalfSize
                           , handleSize
                           , handleSize );

        iBLContext.setFillStyle( BLRgba32( 0xFF808080 ) );
        iBLContext.fillRect( point->GetX() + ctrlX - handleHalfSize
                           , point->GetY() + ctrlY - handleHalfSize
                           , handleSize
                           , handleSize );

        iBLContext.fillRect( point->GetX() - ctrlX - handleHalfSize
                           , point->GetY() - ctrlY - handleHalfSize
                           , handleSize
                           , handleSize );
    }
}
/*
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
*/

void
FVectorPathCubic::DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);

        segment->DrawLoops( iBlock, iBLContext, iRoi );
    }
}

void
FVectorPathCubic::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
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
        Fill( iBlock, iBLContext, iRoi );
    }

    DrawLoops( iBlock, iBLContext, iRoi  );

    DrawShapeVariable( iBlock, iBLContext, iRoi );

    if( mIsSelected )
    {
        DrawStructure( iBlock, iBLContext, iRoi );
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

        /*iBLContext.strokePolygon( vertex , 3 );*/
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

    /*iBLContext.strokePolygon( vertex, 3 );*/
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
            if ( prevSegmentVector.DotProduct(segmentVector) < 1.0f )
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
FVectorPathCubic::DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    double mStartRadius = 10.0f;
    double mEndRadius = 0.0f;

    iBLContext.setCompOp( BL_COMP_OP_SRC_OVER );

    // We fill with stroke color because our curve is made of filled shapes.
    iBLContext.setFillStyle(BLRgba32(mStrokeColor));
    iBLContext.setStrokeStyle(BLRgba32(mStrokeColor));

    if( mSegmentList.size() )
    {
        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
            FVectorSegmentCubic* prevSegment = static_cast<FVectorSegmentCubic*>( segment->GetPreviousSegment() );
            double segmentStartRadius = segment->GetPoint(0).GetRadius();
            FRectD clip = iRoi & segment->GetBoundingBox();

            if( ( iRoi.Area() == 0.0f ) || clip.Area() )
            {
                segment->Draw( iBlock, iBLContext, iRoi );
            }


            if ( prevSegment )
            {
                DrawJoint( iBlock
                         , iBLContext
                         , prevSegment
                         , *segment
                         , segmentStartRadius );
            }
        }
    }
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

        // Note: FVectorPath::AppendPoint() does not create any segment
        FVectorPath::AppendPoint( newPoint );
    }

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegmentCubic* originalSegment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPointCubic* point0 = static_cast<FVectorPointCubic*>( &originalSegment->GetPoint(0) );
        FVectorPointCubic* point1 = static_cast<FVectorPointCubic*>( &originalSegment->GetPoint(1) );
        FVectorSegmentCubic* newSegment = new FVectorSegmentCubic( lookupTable[point0]
                                                                 , originalSegment->GetControlPoint(0).GetX()
                                                                 , originalSegment->GetControlPoint(0).GetY()
                                                                 , originalSegment->GetControlPoint(1).GetX()
                                                                 , originalSegment->GetControlPoint(1).GetY()
                                                                 , lookupTable[point1] );

        FVectorPath::AddSegment( newSegment );

        newSegment->BuildVariable();
    }
}
