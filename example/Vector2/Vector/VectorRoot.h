#pragma once

#include <ULIS>
#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorGroup.h"

class FVectorRoot : public FVectorObject
{
    private:
        FVectorObject* RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double x, double y, double iRadius );

    protected:
        std::list<FVectorObject*> mSelectedObjectList;
        std::list<FVectorObject*> mInvalidatedObjectList;

    public:
        ~FVectorRoot();
        FVectorRoot();
        FVectorRoot( std::string iName );
        void Select( BLContext& iBLContext, double x, double y, double iRadius );
        void Select( BLContext& iBLContext, FVectorObject& iVecObj );
        void ClearSelection();
        FVectorObject* GetLastSelected();
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) { return nullptr; };

        void Bucket( BLContext& iBLContext, double iX, double iY, uint32 iFillColor );
        void InvalidateObject( FVectorObject* iObject );
        void Update();
        FVectorObject* CopyShape();
        FVectorGroup* GroupSelectdObjects();
};
