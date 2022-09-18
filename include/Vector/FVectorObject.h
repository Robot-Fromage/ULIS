#ifndef _FVECTOROBJECT_H_
#define _FVECTOROBJECT_H_

#include <blend2d.h>

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorObject
{
    private:
        virtual void DrawShape( FBlock& iBlock, BLContext& iBLContext ) { };

    protected:
        FVec2D mTranslation;
        double mRotation;
        FVec2D mScaling;
        BLMatrix2D mLocalMatrix;
        BLMatrix2D mWorldMatrix;
        std::list<FVectorObject*> mChildrenList;

    public:
        ~FVectorObject();
        FVectorObject();
        void Draw( FBlock& iBlock, BLContext& iBLContext );
        void DrawChildren( FBlock& iBlock,BLContext& iBLContext );
        void UpdateMatrix( BLContext& iBLContext );
        void Translate( double iX, double iY );
        void Rotate( double iAngle );
        void Scale( double iX, double iY );
        void AddChild( FVectorObject* iChild );
        double GetScalingX();
        double GetScalingY();
};

ULIS_NAMESPACE_END

#endif // _FVECTOROBJECT_H_