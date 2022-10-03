#ifndef _FVECTOROBJECT_H_
#define _FVECTOROBJECT_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

using namespace ::ULIS;

class FVectorObject
{
    protected:
        FVec2D mTranslation;
        double mRotation;
        FVec2D mScaling;
        BLMatrix2D mLocalMatrix;
        BLMatrix2D mWorldMatrix;
        std::list<FVectorObject*> mChildrenList;
        uint32 mStrokeColor;
        double mStrokeWidth;
        uint32 mFillColor;

    public:
        ~FVectorObject();
        FVectorObject();
        virtual void Draw( FBlock& iBlock, BLContext& iBLContext ) final; // cannot be overridden
        virtual bool Pick( BLContext& iBLContext, double iX, double iY ) final; // cannot be overridden
        virtual void DrawShape(FBlock& iBlock,BLContext& iBLContext) = 0;
        virtual bool PickShape(BLContext& iBLContext,double iX,double iY) = 0;
        void DrawChildren( FBlock& iBlock,BLContext& iBLContext );
        void UpdateMatrix( BLContext& iBLContext );
        void Translate( double iX, double iY );
        void Rotate( double iAngle );
        void Scale( double iX, double iY );
        void AddChild( FVectorObject* iChild );
        void RemoveChild( FVectorObject* iChild );
        double GetScalingX();
        double GetScalingY();
        double GetTranslationX();
        double GetTranslationY();
        double GetRotation();
        void CopyTransformation( FVectorObject& iObject );
        BLMatrix2D& GetLocalMatrix();
        std::list<FVectorObject*>& GetChildrenList();
        void SetStrokeColor( uint32 iColor );
        void SetFillColor( uint32 iColor );
        void SetStrokeWidth( double iWidth );
};

#endif // _FVECTOROBJECT_H_