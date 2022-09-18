#ifndef _FVECTORRECTANGLE_H_
#define _FVECTORRECTANGLE_H_

#include <ULIS>
#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

#include "Vector/FVectorObject.h"

class ULIS_API FVectorRectangle : public FVectorObject
{
    private:
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );

    protected :
        double mWidth;
        double mHeight; 

    public:
        ~FVectorRectangle();
        FVectorRectangle();
        FVectorRectangle( double iWidth, double iHeight );
};

ULIS_NAMESPACE_END

#endif // _FVECTORRECTANGLE_H_