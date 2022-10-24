#pragma once

#include <ULIS>
#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"

class FVectorRoot : public FVectorObject
{
    private:
        void RecursiveSelect( BLContext& iBLContext, FVectorObject& iChild, double x, double y, double iRadius );

    protected:
        std::list<FVectorObject*> mSelectedObjectList;

    public:
        ~FVectorRoot();
        FVectorRoot();
        void Select( BLContext& iBLContext, double x, double y, double iRadius );
        void Select( BLContext& iBLContext, FVectorObject& iVecObj );
        void ClearSelection();
        FVectorObject* GetLastSelected();
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi ) {};
        bool PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) { return false; };
};
