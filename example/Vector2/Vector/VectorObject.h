#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

using namespace ::ULIS;

class FVectorRoot;

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
        bool mIsInvalidated;
        FRectD mBBox;
        std::string mName;

    public:
        ~FVectorObject();
        FVectorObject();
        FVectorObject( std::string iName );
        void CopySettings( FVectorObject& iDestinationObject );
        virtual void Update() final ; // cannot be overridden
        virtual void UpdateShape() = 0;
        virtual FVectorObject* Copy() final ; // cannot be overridden
        virtual FVectorObject* CopyShape() = 0;
        virtual void Draw( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi ) final; // cannot be overridden
        virtual void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &roi ) = 0;
        virtual FVectorObject* Pick( BLContext& iBLContext, double iX, double iY, double iRadius ) final; // cannot be overridden
        virtual FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) = 0;
        /*virtual void UpdateBoundingBox() = 0;*/
        void DrawChildren( FBlock& iBlock,BLContext& iBLContext, FRectD& iRoi );
        void UpdateMatrix( BLContext& iBLContext );
        void Translate( double iX, double iY );
        void Rotate( double iAngle );
        void Scale( double iX, double iY );
        void PrependChild( FVectorObject* iChild );
        void AppendChild( FVectorObject* iChild );
        void AddChild( FVectorObject* iChild, bool iPrepend );
        void RemoveChild( FVectorObject* iChild );
        void ImportChild( FVectorObject* iChild, BLMatrix2D& iInverseWorldMatrix );
        static void ExtractTransformations( BLMatrix2D &iMatrix, FVec2D* iTranslation, double* iRotation, FVec2D* iScaling );
        double GetScalingX();
        double GetScalingY();
        double GetTranslationX();
        double GetTranslationY();
        double GetRotation();
        FVectorObject* GetParent();
        void SetParent( FVectorObject* iObject );
        void CopyTransformation( FVectorObject& iObject );
        BLMatrix2D& GetLocalMatrix();
        BLMatrix2D& GetWorldMatrix();
        BLMatrix2D& GetInverseWorldMatrix();
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
        void Invalidate();
        FVectorRoot* GetRoot();
        bool IsInvalidated();
        bool IsSelected();
};
