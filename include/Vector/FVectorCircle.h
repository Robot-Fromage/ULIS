#ifndef _FVECTORCIRCLE_H_
#define _FVECTORCIRCLE_H_

#include <ULIS>
#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

#include "Vector/FVectorObject.h"

class ULIS_API FVectorCircle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );

    protected :
        double mRadius; 

    public:
        ~FVectorCircle();
        FVectorCircle();
        FVectorCircle( double iRadius );
};

ULIS_NAMESPACE_END

#endif // _FVECTORCIRCLE_H_