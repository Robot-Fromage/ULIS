#ifndef _FVECTORSEGMENTQUADRATIC_H_
#define _FVECTORSEGMENTQUADRATIC_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include <Vector/FVectorPoint.h>
#include <Vector/FVectorSegment.h>

ULIS_NAMESPACE_BEGIN

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

ULIS_NAMESPACE_END

#endif // _FVECTORSEGMENTQUADRATIC_H_