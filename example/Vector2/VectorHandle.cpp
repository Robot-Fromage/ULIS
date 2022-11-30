#include <blend2d.h>
#include "Vector/Vector.h"

FVectorHandle::~FVectorHandle()
{
}

FVectorHandle::FVectorHandle()
    : FVectorPoint()
{
}

FVectorHandle::FVectorHandle( double iX, double iY )
    : FVectorPoint( iX, iY )
{

}
