#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorLink
{
    protected:
        FVectorPoint* mPoint[2];

    public:
        ~FVectorLink();
        FVectorLink( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVec2D GetVector( bool iNormalize );
        FVectorPoint* GetPoint( int iPointNum );
        virtual double GetDistanceSquared();
        virtual FVec2D GetPointAt( double t );
        double GetStraightDistance();
};
