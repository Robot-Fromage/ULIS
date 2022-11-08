#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathLoop::~FVectorPathLoop()
{
}

FVectorPathLoop::FVectorPathLoop()
    : FVectorPath()
{

}

FVectorPathLoop::FVectorPathLoop( std::list<FVectorPoint*> iLoopPointist )
    : FVectorPath()
{
    ShapeFromPointList ( iLoopPointist );
}

void
FVectorPathLoop::ShapeFromPointList( std::list<FVectorPoint*> iLoopPointist )
{
   FVectorPoint* firstPoint = nullptr;

    for( std::list<FVectorPoint*>::iterator it = iLoopPointist.begin(); it != iLoopPointist.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        AppendPoint( point );
    }
}

static void DrawSegmentCubic( BLPath& iPath
                            , FVectorSegmentCubic& iSegment
                            , double iFromT
                            , double iToT )
{
    std::vector<FPolygon>& polygonCache = iSegment.GetPolygonCache();
    uint32 polyCount = iSegment.GetPolygonCount();
    bool revert = ( iFromT < iToT ) ? false : true;

    if ( revert == false )
    {
        // TODO: find why iSegment.GetPolygonCount() differs from polygonCache.size()
        for( int i = 0; i < polyCount; i++ )
        {
            if( polygonCache[i].toT > iFromT && polygonCache[i].fromT < iToT )
            {
                BLPoint to = { polygonCache[i].lineVertex[1].x, polygonCache[i].lineVertex[1].y };

                iPath.lineTo ( to.x, to.y );
            }
        }
    }
    else
    {
        double tmp = iFromT;

        iFromT = iToT;
        iToT = iFromT;

        for( int i = polyCount - 1; i >= 0; i-- )
        {
            /*if( polygonCache[i].toT > iFromT && polygonCache[i].fromT < iToT )
            {*/
                BLPoint to = { polygonCache[i].lineVertex[0].x, polygonCache[i].lineVertex[0].y };

                iPath.lineTo ( to.x, to.y );
            /*}*/
        }
    }
/*
    // TODO: find why iSegment.GetPolygonCount() differs from polygonCache.size()
    for ( int i = 0; i < iSegment.GetPolygonCount(); i++ )
    {
        if ( polygonCache[i].toT >= iFromT )
        {
            BLPoint to = { polygonCache[i].lineVertex[1].x, polygonCache[i].lineVertex[1].y };

            // clipping part
            if ( iToT < polygonCache[i].toT )
            {
                FVec2D dir = polygonCache[i].lineVertex[1] - polygonCache[i].lineVertex[0];

                to.x = polygonCache[i].lineVertex[0].x + dir.x * ( iToT - polygonCache[i].fromT );
                to.y = polygonCache[i].lineVertex[0].y + dir.y * ( iToT - polygonCache[i].fromT );
            }

            iPath.lineTo ( to.x, to.y );
        }
    }
*/
}

void
FVectorPathLoop::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    BLPath path;

    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF8000 ) );
    iBLContext.setFillStyle( BLRgba32( 0xFFFF8000 ) );

    if ( mPointList.size() ) 
    {
        FVectorPoint* firstPoint = mPointList.front();
        FVectorPoint* lastPoint = mPointList.back();
        std::list<FVectorPoint*>::iterator     it;
        std::list<FVectorPoint*>::iterator nextit;

        for( it = mPointList.begin(), nextit = ++mPointList.begin(); it != mPointList.end(); ++it )
        {
            FVectorPoint* point = static_cast<FVectorPoint*>(*it);
            FVectorPoint* nextPoint = ( point == lastPoint ) ? firstPoint : static_cast<FVectorPoint*>(*nextit++);

            if ( point == firstPoint ) {
                FVec2D pointAt = static_cast<FVectorPointIntersection*>(point)->GetPosition(*point->GetFirstSegment());

                path.moveTo ( pointAt.x, pointAt.y );
            }

/*
                if ( (     point->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
                  && ( nextPoint->GetType() == FVectorPoint::POINT_TYPE_REGULAR      ) )
                {
                    FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(point);
                    FVectorSegment* segment = intersectionPoint->GetSegment( *nextPoint );
                    FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(segment);

                    DrawSegmentCubic ( path, *cubicSegment, intersectionPoint->GetT(*segment), 1.0f );
                }

                if ( (     point->GetType() == FVectorPoint::POINT_TYPE_REGULAR )
                  && ( nextPoint->GetType() == FVectorPoint::POINT_TYPE_REGULAR ) )
                {
                    FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(point->GetSegment( *nextPoint ));

                    if ( segment )
                    {
                        FVec2D& point0 = segment->GetPoint(0).GetCoords();
                        FVec2D& point1 = segment->GetPoint(1).GetCoords();
                        FVec2D& ctrlPoint0 = segment->GetControlPoint(0).GetCoords();
                        FVec2D& ctrlPoint1 = segment->GetControlPoint(1).GetCoords();

                        path.cubicTo ( ctrlPoint0.x
                                     , ctrlPoint0.y
                                     , ctrlPoint1.x
                                     , ctrlPoint1.y
                                     , point1.x
                                     , point1.y  );

                    }
                }
*/

                if ( (     point->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
                  && ( nextPoint->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION ) )
                {
                    FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(point->GetSegment( *nextPoint ));

                    if ( cubicSegment )
                    {
                        double     pointT =     static_cast<FVectorPointIntersection*>(point)->GetT(*cubicSegment);
                        double nextPointT = static_cast<FVectorPointIntersection*>(nextPoint)->GetT(*cubicSegment);

                        FVec2D     pointAt = static_cast<FVectorPointIntersection*>(point)->GetPosition(*cubicSegment);
                        FVec2D nextPointAt = static_cast<FVectorPointIntersection*>(nextPoint)->GetPosition(*cubicSegment);

/*iBLContext.strokeLine( pointAt.x, pointAt.y, nextPointAt.x, nextPointAt.y );*/
                        DrawSegmentCubic ( path, *cubicSegment, nextPointT, pointT );
                    }
                }

/*
                if ( (     point->GetType() == FVectorPoint::POINT_TYPE_REGULAR      )
                  && ( nextPoint->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION ) )
                {
                    FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(nextPoint);
                    FVectorSegment* segment = intersectionPoint->GetSegment(*point);
                    FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(segment);

                    DrawSegmentCubic ( path, *cubicSegment, 0.0f, intersectionPoint->GetT(*segment) );
                }
*/
        }
    }


    iBLContext.fillPath( path );

}
