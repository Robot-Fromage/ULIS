#ifndef _FVECTORPOINTCUBIC_H_
#define _FVECTORPOINTCUBIC_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorPointCubic : public FVectorPoint
{
    private:
        FVectorPointControl mCtrlPoint;

    protected:

    public:
        ~FVectorPointCubic();
        FVectorPointCubic();
        FVectorPointCubic( double iX, double iY );
        FVectorPointCubic( double iX, double iY, double iRadius );
        FVectorPointControl& GetControlPoint();
        double GetControlPointDistance();
};

#endif // _FVECTORPOINTCUBIC_H_