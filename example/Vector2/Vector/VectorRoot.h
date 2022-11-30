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
        FVectorObject* RecursiveSelect( FVectorObject& iObj, double x, double y, double iRadius );
        void UpdateShape();
        FVectorObject* CopyShape();

    protected:
        std::list<FVectorObject*> mSelectedObjectList;
        std::list<FVectorObject*> mInvalidatedObjectList;

    public:
        ~FVectorRoot();
        FVectorRoot( std::string iName );
        void Select( double x, double y, double iRadius );
        void Select( FVectorObject& iVecObj );
        void ClearSelection();
        FVectorObject* GetLastSelected();
        void DrawShape( FRectD& iRoi, uint64 iFlags );
        FVectorObject* PickShape( double iX, double iY, double iRadius ) { return nullptr; };

        void Bucket( double iX, double iY, uint32 iFillColor );
        void InvalidateObject( FVectorObject* iObject );

        FVectorGroup* GroupSelectdObjects();
};
