#ifndef _FVECTORPOINT_H_
#define _FVECTORPOINT_H_

#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

class FVectorSegment;

class FVectorPoint
{
    private:

    protected:
        double mX;
        double mY;

        std::vector<FVectorSegment*> mSegmentList;
    public:
        ~FVectorPoint();
        FVectorPoint();
        FVectorPoint( double iX,double iY );
        void AddSegment( FVectorSegment* iSegment );
        double GetX();
        double GetY();
        void SetX( double iX );
        void SetY( double iY );
        void Set( double iX, double iY );

};

#endif // _FVECTORPOINT_H_