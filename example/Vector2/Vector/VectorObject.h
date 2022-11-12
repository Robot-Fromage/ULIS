#pragma once

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
        BLMatrix2D mInverseLocalMatrix;
        BLMatrix2D mWorldMatrix;
        BLMatrix2D mInverseWorldMatrix;
        std::list<FVectorObject*> mChildrenList;
        uint32 mStrokeColor;
        double mStrokeWidth;
        uint32 mFillColor;
        FVectorObject* mParent;
        bool mIsFilled;
        bool mIsSelected;
        FRectD mBBox;
        std::string mName;

    public:
        ~FVectorObject();
        FVectorObject();
        FVectorObject( std::string iName );
        virtual void Draw( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi ) final; // cannot be overridden
        virtual FVectorObject* Pick( BLContext& iBLContext, double iX, double iY, double iRadius ) final; // cannot be overridden
        virtual void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &roi ) = 0;
        virtual FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) = 0;
        /*virtual void UpdateBoundingBox() = 0;*/
        void DrawChildren( FBlock& iBlock,BLContext& iBLContext, FRectD& iRoi );
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
        FVectorObject* GetParent();
        void CopyTransformation( FVectorObject& iObject );
        BLMatrix2D& GetLocalMatrix();
        BLMatrix2D& GetWorldMatrix();
        std::list<FVectorObject*>& GetChildrenList();
        void SetStrokeColor( uint32 iColor );
        void SetFillColor( uint32 iColor );
        void SetFilled(bool iIsFilled);
        void SetStrokeWidth( double iWidth );
        double GetStrokeWidth( );
        FVec2D WorldCoordinatesToLocal( double iX, double iY );
        void SetIsSelected( bool iIsSelected );
        FRectD GetBBox( bool iWorld );
        bool IsFilled();
        void MoveBack();
        void MoveFront();
};
