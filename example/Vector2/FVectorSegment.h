#ifndef _FVECTORSEGMENT_H_
#define _FVECTORSEGMENT_H_

#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

#include "FVectorPoint.h"

class FVectorSegment
{
protected:
    FVectorPoint *mPoint[2];

public:
    ~FVectorSegment();
    FVectorSegment();
    FVectorSegment(FVectorPoint* iPoint0,FVectorPoint* iPoint1);
    FVectorPoint* GetPoint( int iPointNum );
    virtual void Draw(FBlock& iBlock,BLContext& iBLContext) = 0;
};

#endif //  _FVECTORSEGMENT_H_