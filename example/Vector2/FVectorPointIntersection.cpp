#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPointIntersection::~FVectorPointIntersection()
{
}

FVectorPointIntersection::FVectorPointIntersection()
{

}

FVectorPointIntersection::FVectorPointIntersection( double iX, double iY )
    : FVectorPoint ( iX, iY )
{
    
}

uint32
FVectorPointIntersection::GetType()
{
    return FVectorPoint::POINT_TYPE_INTERSECTION; // default type;
}
