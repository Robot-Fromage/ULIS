#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointCubic::~FVectorPointCubic()
{
}

FVectorPointCubic::FVectorPointCubic( double iX,double iY )
    : FVectorPoint( iX, iY )
    , mCtrlPoint ( *this )
{
    SetRadius( 1.0f, false );
}

FVectorPointCubic::FVectorPointCubic()
    : FVectorPointCubic( 0.0f, 0.0f )
{
}

FVectorPointCubic::FVectorPointCubic( double iX, double iY, double iRadius )
    : FVectorPointCubic ( iX, iY )
{
    SetRadius( iRadius, false );
}

FVectorHandlePoint&
FVectorPointCubic::GetControlPoint()
{
    return mCtrlPoint;
}

void
FVectorPointCubic::Set( double iX
                      , double iY
                      , bool iBuildSegments )
{
    FVectorPoint::Set( iX, iY );

    if( iBuildSegments == true )
    {
        BuildSegments();
    }
}

void
FVectorPointCubic::Set( double iX
                      , double iY )
{
    FVectorPointCubic::Set( iX, iY, true );
}

void
FVectorPointCubic::Set( double iX
                      , double iY
                      , double iRadius
                      , bool iBuildSegments )
{
    SetRadius( iRadius, false );
    Set( iX, iY, iBuildSegments );
}

void
FVectorPointCubic::SetRadius( double iRadius, bool iBuildSegments )
{
    FVectorPoint::SetRadius ( iRadius, iBuildSegments );

    if ( iBuildSegments == true )
    {
        BuildSegments();
    }
}

void
FVectorPointCubic::BuildSegments()
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegmentCubic* segment = static_cast<FVectorSegmentCubic*>(*it);

        segment->BuildVariable();
    }
}

FVec2D
FVectorPointCubic::GetPerpendicularVector( bool iNormalize )
{
    FVec2D parallel = { 0.0f, 0.0f };
    FVec2D perpendicular = { 0.0f, 0.0f };

    if ( mSegmentList.size() )
    {
        uint32 count = 0;

        for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
        {
            FVectorSegmentCubic *segment = static_cast<FVectorSegmentCubic*>(*it);
            FVectorPoint* p0 = segment->GetPoint(0);
            FVectorPoint* p1 = segment->GetPoint(1);
            FVec2D& point0 = segment->GetPoint(0)->GetCoords();
            FVec2D& point1 = segment->GetPoint(1)->GetCoords();
            FVec2D& ctrlPoint0 = segment->GetControlPoint(0).GetCoords();
            FVec2D& ctrlPoint1 = segment->GetControlPoint(1).GetCoords();

            if( this == p0 )
            {
                // this is less computation-heavy
                FVec2D vec = { ctrlPoint0 - point0 };
                /*FVec2D vec =  { CubicBezierTangentAtParameter<FVec2D>( point0.GetCoords()
                                                                     , ctrlPoint0.GetCoords()
                                                                     , ctrlPoint1.GetCoords()
                                                                     , point1.GetCoords()
                                                                     , 0.0f ) };*/

                if ( vec.DistanceSquared() )
                {
                    vec.Normalize();

                    parallel.x += vec.x;
                    parallel.y += vec.y;
                }
            }

            if( this == p1 )
            {
                FVec2D vec = { point1 - ctrlPoint1 };
                /*FVec2D vec =  { CubicBezierTangentAtParameter<FVec2D>( point0.GetCoords()
                                                                     , ctrlPoint0.GetCoords()
                                                                     , ctrlPoint1.GetCoords()
                                                                     , point1.GetCoords()
                                                                     , 1.0f ) };*/

                if ( vec.DistanceSquared() )
                {
                    vec.Normalize();

                    parallel.x += vec.x;
                    parallel.y += vec.y;
                }
            }
        }

        if ( iNormalize == true )
        {
            if ( parallel.DistanceSquared() )
            {
                parallel.Normalize();
            }

            perpendicular.x =   parallel.y;
            perpendicular.y = - parallel.x;
        }
    }

    return perpendicular;
}