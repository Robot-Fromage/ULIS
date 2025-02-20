#ifndef _FVECTORPOINT_H_
#define _FVECTORPOINT_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include <Vector/FVectorSegment.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorPoint
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

ULIS_NAMESPACE_END

#endif // _FVECTORPOINT_H_