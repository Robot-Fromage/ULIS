#ifndef _FVECTOROBJECT_H_
#define _FVECTOROBJECT_H_

#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

class FVectorObject
{
    public:
        ~FVectorObject();
        FVectorObject();
        virtual void Draw(FBlock& iBlock,BLContext& blctx) = 0;
};

#endif // _FVECTOROBJECT_H_