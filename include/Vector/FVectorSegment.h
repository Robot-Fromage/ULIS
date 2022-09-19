#ifndef _FVECTORSEGMENT_H_
#define _FVECTORSEGMENT_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorPoint;

class ULIS_API FVectorSegment
{
protected:
    FVectorPoint *mPoint[2];

public:
    ~FVectorSegment();
    FVectorSegment();
    FVectorSegment( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
    FVectorPoint* GetPoint( int iPointNum );
    virtual void Draw( FBlock& iBlock, BLContext& iBLContext ) = 0;
};

ULIS_NAMESPACE_END

#endif //  _FVECTORSEGMENT_H_