#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorPointCubic : public FVectorPoint
{
    private:
        FVectorHandlePoint mCtrlPoint;
        double mRadius;

    protected:

    public:
        ~FVectorPointCubic();
        FVectorPointCubic();
        FVectorPointCubic( double iX, double iY );
        FVectorPointCubic( double iX, double iY, double iRadius );
        FVectorHandlePoint& GetControlPoint();
        double GetRadius();
        void SetRadius( double iRadius );
        FVec2D GetPerpendicularVector();
};
