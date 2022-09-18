#ifndef _FVECTORSEGMENT_H_
#define _FVECTORSEGMENT_H_

#include <ULIS>
#include <blend2d.h>

#include "Vector/FVectorPoint.h"

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorSegment
{
protected:
    FVectorPoint *mPoint[2];

public:
    ~FVectorSegment();
    FVectorSegment();
    FVectorSegment(FVectorPoint* iPoint0,FVectorPoint* iPoint1);
    FVectorPoint* GetPoint( int iPointNum );
    virtual void Draw( FBlock& iBlock,BLContext& iBLContext ) = 0;
};

ULIS_NAMESPACE_END

#endif //  _FVECTORSEGMENT_H_