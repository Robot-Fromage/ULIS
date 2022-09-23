#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathBuilder::~FVectorPathBuilder()
{
}

FVectorPathBuilder::FVectorPathBuilder()
    : FVectorPath()
{

}

FVectorSegment*
FVectorPathBuilder::AppendPoint( FVectorPoint* iPoint )
{
    FVectorPoint* lastPoint = GetLastPoint();

    FVectorPath::AppendPoint( iPoint );

    // lastPoint is NULL if this is the first point added
    if( lastPoint )
    {
        FVectorSegment* segment = new FVectorSegment( lastPoint, iPoint );

        AddSegment( segment );

        return segment;
    }

    return NULL;
}


void
FVectorPathBuilder::DrawShape( FBlock& iBlock, BLContext& iBLContext )
{
    iBLContext.setCompOp(BL_COMP_OP_SRC_COPY);
    /*iBLContext.setFillStyle(BLRgba32(0xFFFFFFFF));
    iBLContext.setStrokeStyle(BLRgba32(0xFF000000));*/

    for(std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it)
    {
        FVectorSegment *segment = (*it);

        segment->Draw( iBlock, iBLContext );
    }
}

void
FVectorPathBuilder::Pick( double iX
                        , double iY
                        , double iRadius )
{
    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPoint* point = *it;

        if( ( fabs( point->GetX() - iX ) <= iRadius ) &&
            ( fabs( point->GetY() - iY ) <= iRadius ) )
        {
            mSelectedPointList.push_back( point );
        }
    }
}

void
FVectorPathBuilder::Unselect(FVectorPoint *iPoint)
{
    if( iPoint == NULL )
    {
        mSelectedPointList.clear();
    } else
    {
        mSelectedPointList.remove(iPoint);
    }
}
