#include <blend2d.h>
#include "Vector/Vector.h"

FVectorLink::~FVectorLink()
{

}

FVectorLink::FVectorLink( FVectorPoint* iPoint0
                        , FVectorPoint* iPoint1 )
    : mPoint { iPoint0, iPoint1 }
{

}

FVectorPoint*
FVectorLink::GetPoint( int iPointNum )
{
    return mPoint[iPointNum];
}

double
FVectorLink::GetStraightDistance()
{
    FVec2D vec = { mPoint[1]->GetX() - mPoint[0]->GetX(),
                   mPoint[1]->GetY() - mPoint[0]->GetY() };

    return vec.Distance();
}

FVec2D
FVectorLink::GetVector( bool iNormalize )
{
    FVec2D vec = mPoint[1]->GetCoords() - mPoint[0]->GetCoords();

    if ( iNormalize == true )
    {
        if ( vec.DistanceSquared() )
        {
            vec.Normalize();
        }
    }

    return vec;
}

double
FVectorLink::GetDistanceSquared()
{
    FVec2D dist = mPoint[1]->GetCoords() - mPoint[0]->GetCoords();

    return dist.DistanceSquared();
}

FVec2D
FVectorLink::GetPointAt ( double t )
{
    FVec2D& p0 =  mPoint[0]->GetCoords();
    FVec2D& p1 =  mPoint[0]->GetCoords();
    FVec2D vec = p1 - p0;

    return p0 + ( vec * t );
}
