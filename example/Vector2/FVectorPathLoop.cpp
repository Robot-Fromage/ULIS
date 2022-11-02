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
    for( std::list<FVectorPoint*>::iterator it = iLoopPointist.begin(); it != iLoopPointist.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        AppendPoint( point );
    }
}

void
FVectorPathLoop::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    BLPath path;
    FVectorPoint* prevPoint = nullptr;

    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        if ( prevPoint == nullptr )
        {
            path.moveTo ( point->GetX(), point->GetY() );
        }
        else
        {
            // we are on a point that belongs to segment (not a control point nor an intersection)
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

                    path.lineTo ( point0.x, point0.y );
                    path.cubicTo ( ctrlPoint0.x
                                 , ctrlPoint0.y
                                 , ctrlPoint1.x
                                 , ctrlPoint1.y
                                 , point1.x
                                 , point1.y  );
                }
            } else {
                path.lineTo ( point->GetX(), point->GetY() );
            }
        }

        prevPoint = point;
    }

    iBLContext.setFillStyle( BLRgba32( 0xFFFF8000 ) );
    iBLContext.fillPath( path );
}
