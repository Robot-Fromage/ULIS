#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointIntersection::~FVectorPointIntersection()
{
}

FVectorPointIntersection::FVectorPointIntersection()
    : FVectorPoint ( 0.0f, 0.0f )
{

}

uint32
FVectorPointIntersection::GetType()
{
    return FVectorPoint::POINT_TYPE_INTERSECTION; // default type;
}

double
FVectorPointIntersection::GetT( FVectorSegment& iSegment )
{
    return mTMap[&iSegment].t;
}

FVec2D
FVectorPointIntersection::GetPosition( FVectorSegment& iSegment )
{
    return mTMap[&iSegment].position;
}

FVec2D&
FVectorPointIntersection::GetCoords()
{
    return mTMap[GetFirstSegment()].position;
}

void
FVectorPointIntersection::Draw( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        FVec2D position = GetPosition(*segment);

        iBLContext.setFillStyle( BLRgba32( 0xFFFF80FF) );
        iBLContext.fillRect( position.x - 5, position.y - 5, 10, 10 );
    }
}

void
FVectorPointIntersection::AddSegment( FVectorSegmentCubic* iSegment, double t )
{
    FVec2D& point0 = iSegment->GetPoint(0)->GetCoords();
    FVec2D& point1 = iSegment->GetPoint(1)->GetCoords();
    FVec2D& ctrlPoint0 = iSegment->GetControlPoint(0).GetCoords();
    FVec2D& ctrlPoint1 = iSegment->GetControlPoint(1).GetCoords();
    FVec2D intersectAt = CubicBezierPointAtParameter<FVec2D>( point0
                                                            , ctrlPoint0
                                                            , ctrlPoint1
                                                            , point1
                                                            , t );
    FIntersection intersect = { intersectAt, t };
/*printf("%f %f %f\n", intersectAt.x, intersectAt.y, t);*/
    FVectorPoint::AddSegment(iSegment);

    mTMap.insert(std::make_pair(iSegment, intersect));
}

FVectorSegment*
FVectorPointIntersection::GetSegment( FVectorPoint& iOtherPoint )
{
    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( iOtherPoint.GetType() == POINT_TYPE_REGULAR )
        {
            if ( ( segment->GetPoint(0) == &iOtherPoint )
              || ( segment->GetPoint(1) == &iOtherPoint ) ) {
                return segment;
            }
        }

        if ( iOtherPoint.GetType() == POINT_TYPE_INTERSECTION )
        {
            if ( segment->HasIntersectionPoint ( static_cast<FVectorPointIntersection&>(iOtherPoint) ) )
            {
                return segment;
            }
        }
    }

    return nullptr;
}
