#ifndef _FVECTOROBJECT_H_
#define _FVECTOROBJECT_H_

#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorObject
{
    public:
        ~FVectorObject();
        FVectorObject();
        virtual void Draw( FBlock& iBlock, BLContext& blctx ) = 0;
};

ULIS_NAMESPACE_END

#endif // _FVECTOROBJECT_H_