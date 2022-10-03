#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointControl::~FVectorPointControl()
{
}

FVectorPointControl::FVectorPointControl()
    : FVectorPoint()
{
}

FVectorPointControl::FVectorPointControl( double iX, double iY )
    : FVectorPoint( iX, iY )
{
    Set( iX, iY );
}
