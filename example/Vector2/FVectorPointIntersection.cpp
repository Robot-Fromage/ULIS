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

void
FVectorPointIntersection::AddLoop( FVectorPathLoop* iLoop )
{
    mLoopList.push_back( iLoop );
}

void
FVectorPointIntersection::DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    for( std::list<FVectorPathLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorPathLoop* loop = static_cast<FVectorPathLoop*>(*it);

        loop->DrawShape( iBlock, iBLContext, iRoi );
    }
}

void
FVectorPointIntersection::AddSegment( FVectorSegmentCubic* iSegment, double t )
{
    FVec2D& point0 = iSegment->GetPoint(0).GetCoords();
    FVec2D& point1 = iSegment->GetPoint(1).GetCoords();
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

        if ( ( &segment->GetPoint(0) == &iOtherPoint )
          || ( &segment->GetPoint(1) == &iOtherPoint ) ) {
            return segment;
        }
    }

    return nullptr;
}
