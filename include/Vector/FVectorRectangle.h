#ifndef _FVECTORRECTANGLE_H_
#define _FVECTORRECTANGLE_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include <Vector/FVectorObject.h>
#include <Vector/FVectorSegment.h>

ULIS_NAMESPACE_BEGIN


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