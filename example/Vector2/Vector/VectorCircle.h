#ifndef _FVECTORCIRCLE_H_
#define _FVECTORCIRCLE_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorCircle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        bool PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );

    protected :
        double mRadius; 

    public:
        ~FVectorCircle();
        FVectorCircle();
        FVectorCircle( double iRadius );
        void SetRadius( double iRadius );
        double GetRadius( );
};

#endif // _FVECTORCIRCLE_H_