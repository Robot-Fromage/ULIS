#ifndef _FVECTORSEGMENTCUBIC_H_
#define _FVECTORSEGMENTCUBIC_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include <Vector/FVectorPoint.h>
#include <Vector/FVectorSegment.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorSegmentCubic : public FVectorSegment
{
    private:
        FVectorPoint mCtrlPoint[2];
    public:
        ~FVectorSegmentCubic();
        FVectorSegmentCubic( );
        FVectorSegmentCubic( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorSegmentCubic( FVectorPoint* iPoint0, double iCtrlPoint0x, double iCtrlPoint0y, double iCtrlPoint1x, double iCtrlPoint1y, FVectorPoint* iPoint1 );
        FVectorPoint* GetControlPoint( int iCtrlPointNum );
        void Draw( FBlock& iBlock, BLContext& iBLContext );
};

ULIS_NAMESPACE_END

#endif // _FVECTORSEGMENTCUBIC_H_