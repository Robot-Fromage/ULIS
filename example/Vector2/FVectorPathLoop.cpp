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

    // TODO: find why iSegment.GetPolygonCount() differs from polygonCache.size()
    for ( int i = 0; i < iSegment.GetPolygonCount(); i++ )
    {
        if ( ( polygonCache[i].toT >= iFromT ) &&  ( polygonCache[i].fromT <= iToT ) )
        {
            BLPoint pt[2] = { { polygonCache[i].lineVertex[0].x, polygonCache[i].lineVertex[0].y }
                            , { polygonCache[i].lineVertex[1].x, polygonCache[i].lineVertex[1].y } };

            iPath.lineTo ( pt[1].x, pt[1].y );
        }
    }
}

void
FVectorPathLoop::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    BLPath path;
    FVectorPoint* prevPoint = nullptr;

    if ( mPointList.size() ) 
    {
        FVectorPoint* firstPoint = mPointList.front();

        for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
        {
            FVectorPoint* point = static_cast<FVectorPoint*>(*it);

            if ( prevPoint == nullptr )
            {

                // the first point is always an intersection point in our model
                if ( point->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
                {
                    FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(point);
                    FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(point->GetFirstSegment());
                    FVec2D intersectAt = intersectionPoint->GetPosition(*segment);

                    path.moveTo ( intersectAt.x, intersectAt.y );
                }
            }
            else
            {
                if ( ( prevPoint->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
                  && (     point->GetType() == FVectorPoint::POINT_TYPE_REGULAR      ) )
                {
                    FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(prevPoint);

                    FVectorSegment* segment = intersectionPoint->GetSegment( *point );
                    FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(segment);

                    DrawSegmentCubic ( path, *cubicSegment, intersectionPoint->GetT(*segment), 1.0f );
                }

                if ( ( prevPoint->GetType() == FVectorPoint::POINT_TYPE_REGULAR )
                  && (     point->GetType() == FVectorPoint::POINT_TYPE_REGULAR ) )
                {
                    FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(prevPoint->GetSegment( *point ));

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
            }

            prevPoint = point;
        }

        if ( ( prevPoint->GetType()  == FVectorPoint::POINT_TYPE_REGULAR      )
          && ( firstPoint->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION ) )
        {
            FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(firstPoint);

            FVectorSegment* segment = intersectionPoint->GetSegment( *prevPoint );
            FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(segment);

            DrawSegmentCubic ( path, *cubicSegment, 0.0f, intersectionPoint->GetT(*segment) );
        }

    }

    iBLContext.setFillStyle( BLRgba32( 0xFFFF8000 ) );
    iBLContext.fillPath( path );
}
