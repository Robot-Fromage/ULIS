#ifndef _FVECTORPOINTCONTROL_H_
#define _FVECTORPOINTCONTROL_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"

class FVectorPointControl : public FVectorPoint
{
    private:

    protected:

    public:
        ~FVectorPointControl();
        FVectorPointControl();
        FVectorPointControl( double iX, double iY );
};

#endif // _FVECTORPOINTCONTROL_H_