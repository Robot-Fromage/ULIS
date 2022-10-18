#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointCubic::~FVectorPointCubic()
{
}

FVectorPointCubic::FVectorPointCubic()
    : FVectorPoint( 0.0f, 0.0f )
{
    mCtrlPoint.Set( 1.0f, 1.0f );
}

FVectorPointCubic::FVectorPointCubic( double iX,double iY )
    : FVectorPoint( iX, iY )
{
    mCtrlPoint.Set( 1.0f, 1.0f );
}

FVectorPointCubic::FVectorPointCubic( double iX, double iY, double iRadius )
    : FVectorPoint( iX, iY )
{
    mCtrlPoint.Set( iRadius, iRadius );
}

FVectorPointControl& GetControlPoint();
double GetControlPointDistance();

FVectorPointControl&
FVectorPointCubic::GetControlPoint()
{
    return mCtrlPoint;
}

double
FVectorPointCubic::GetControlPointDistance()
{
    FVec2D vec = { mCtrlPoint.GetX(), mCtrlPoint.GetY() };

    return vec.Distance();
}

FVec2D
FVectorPointCubic::GetPerpendicularVector()
{
    FVectorSegmentCubic* seg0 = mSegmentList.front();
    FVectorSegmentCubic* seg1 = mSegmentList.back();

    if ( seg0 )
    {
        if ( seg0->GetPoint() )
    }

    if( seg1 && seg1 != seg0 )
    {

    }
}