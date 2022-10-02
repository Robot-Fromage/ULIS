#ifndef _FVECTORPOINT_H_
#define _FVECTORPOINT_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorSegment.h"

class FVectorPoint
{
    private:

    protected:
        double mX;
        double mY;

        std::list<FVectorSegment*> mSegmentList;
    public:
        ~FVectorPoint();
        FVectorPoint();
        FVectorPoint( double iX,double iY );
        void AddSegment( FVectorSegment* iSegment );
        void RemoveSegment( FVectorSegment* iSegment );
        double GetX();
        double GetY();
        void SetX( double iX );
        void SetY( double iY );
        void Set( double iX, double iY );
        uint32 GetSegmentCount();

};

#endif // _FVECTORPOINT_H_