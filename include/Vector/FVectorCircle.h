#ifndef _FVECTORCIRCLE_H_
#define _FVECTORCIRCLE_H_

#include <ULIS>
#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorCircle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        bool PickShape( BLContext& iBLContext, double iX, double iY );

    protected :
        double mRadius; 

    public:
        ~FVectorCircle();
        FVectorCircle();
        FVectorCircle( double iRadius );
};

ULIS_NAMESPACE_END

#endif // _FVECTORCIRCLE_H_