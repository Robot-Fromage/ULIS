#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorPointCubic : public FVectorPoint
{
    private:
        FVectorHandlePoint mCtrlPoint;

    protected:

    public:
        ~FVectorPointCubic();
        FVectorPointCubic();
        FVectorPointCubic( double iX, double iY );
        FVectorPointCubic( double iX, double iY, double iRadius );
        FVectorHandlePoint& GetControlPoint();
        void Set( double iX, double iY );
        void Set( double iX, double iY, bool iBuildSegments );
        void Set( double iX, double iY, double iRadius, bool iBuildSegments );
        FVec2D GetPerpendicularVector( bool iNormalize );
        void BuildSegments();
        virtual void SetRadius( double iRadius, bool iBuildSegments );
        void SmoothSegments( bool iBuildSegments );
};
