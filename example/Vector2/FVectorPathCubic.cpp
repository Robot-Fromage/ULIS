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

    FVectorPath::AppendPoint(iPoint);

    // lastPoint is NULL if this is the first point added
    if(lastPoint)
    {
        FVectorSegmentCubic* segment = new FVectorSegmentCubic( lastPoint, iPoint );

        AddSegment(segment);

        return segment;
    }

    return NULL;
}

void
FVectorPathCubic::Pick( double iX
                      , double iY
                      , double iRadius )
{
    for(std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it)
    {
        FVectorPoint* point = *it;

        if( ( fabs( point->GetX() - iX ) <= iRadius ) &&
            ( fabs( point->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back(point);
        }
    }

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);
        FVectorPoint* ctrlPoint0 = segment->GetControlPoint( 0 );
        FVectorPoint* ctrlPoint1 = segment->GetControlPoint( 1 );

        if( ( fabs( ctrlPoint0->GetX() - iX ) <= iRadius ) &&
            ( fabs( ctrlPoint0->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back(ctrlPoint0);
        }

        if( ( fabs( ctrlPoint1->GetX() - iX ) <= iRadius ) &&
            ( fabs( ctrlPoint1->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back( ctrlPoint1 );
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
