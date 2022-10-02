#ifndef _FVECTORRECTANGLE_H_
#define _FVECTORRECTANGLE_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"

class FVectorRectangle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        bool PickShape( BLContext& iBLContext, double iX, double iY );

    protected :
        double mWidth;
        double mHeight; 

    public:
        ~FVectorRectangle();
        FVectorRectangle();
        FVectorRectangle( double iWidth, double iHeight );
        void SetSize( double iWidth, double iHeight );
        double GetWidth();
        double GetHeight();
};

#endif // _FVECTORRECTANGLE_H_