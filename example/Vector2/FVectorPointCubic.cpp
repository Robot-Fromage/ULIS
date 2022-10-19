#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointCubic::~FVectorPointCubic()
{
}

FVectorPointCubic::FVectorPointCubic( double iX,double iY )
    : FVectorPoint( iX, iY )
{
    mCtrlPoint.SetParent( this );

    SetRadius( 1.0f );
}

FVectorPointCubic::FVectorPointCubic()
    : FVectorPointCubic( 0.0f, 0.0f )
{
}

FVectorPointCubic::FVectorPointCubic( double iX, double iY, double iRadius )
    : FVectorPointCubic ( iX, iY )
{
    SetRadius( iRadius );
}

FVectorHandlePoint&
FVectorPointCubic::GetControlPoint()
{
    return mCtrlPoint;
}

double
FVectorPointCubic::GetRadius()
{
    return mRadius;
}

void
FVectorPointCubic::SetRadius( double iRadius )
{
    mRadius = iRadius;
}

FVec2D
FVectorPointCubic::GetPerpendicularVector()
{
    FVec2D parallel = { 0.0f, 0.0f };
    FVec2D perpendicular = { 0.0f, 0.0f };

    if ( mSegmentList.size() )
    {
        uint32 count = 0;

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            FVectorPoint& point0 = segment->GetPoint(0);
            FVectorPoint& point1 = segment->GetPoint(1);

            if( this == &point0 )
            {
                FVec2D vec =  { segment->GetControlPoint(0).GetX() - segment->GetPoint(0).GetX(),
                                segment->GetControlPoint(0).GetY() - segment->GetPoint(0).GetY() };

                if ( vec.DistanceSquared() )
                {
                    vec.Normalize();

                    parallel.x += vec.x;
                    parallel.y += vec.y;

                    count ++;
                }
            }

            if( this == &point1 )
            {
                FVec2D vec =  { segment->GetPoint(1).GetX() - segment->GetControlPoint(1).GetX(),
                                segment->GetPoint(1).GetY() - segment->GetControlPoint(1).GetY() };

                if ( vec.DistanceSquared() )
                {
                    vec.Normalize();

                    parallel.x += vec.x;
                    parallel.y += vec.y;

                    count ++;
                }
            }
        }

        if ( count )
        {
            parallel /= count;

            parallel.Normalize();

            perpendicular.x =   parallel.y;
            perpendicular.y = - parallel.x;
        }
    }

    return perpendicular;
}