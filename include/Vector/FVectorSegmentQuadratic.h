#ifndef _FVECTORSEGMENTQUADRATIC_H_
#define _FVECTORSEGMENTQUADRATIC_H_

#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

#include "FVectorSegment.h"

class ULIS_API FVectorSegmentQuadratic : public FVectorSegment
{
    private:
        FVectorPoint mCtrlPoint;

    public:
        ~FVectorSegmentQuadratic();
        FVectorSegmentQuadratic();
        FVectorSegmentQuadratic( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorSegmentQuadratic( FVectorPoint* iPoint0, double iCtrlx, double iCtrly, FVectorPoint* iPoint1 );
        void Draw( FBlock& iBlock, BLContext& iBLContext );
};

#endif // _FVECTORSEGMENTQUADRATIC_H_