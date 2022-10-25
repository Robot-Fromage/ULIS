#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"

class FVectorPath : public FVectorObject
{
    protected :
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorSegment*> mSegmentList;
        std::list<FVectorPoint*> mSelectedPointList;
        void AddSegment( FVectorSegment* iSegment );
        void RemoveSegment( FVectorSegment* iSegment );
        BLPath mPath;

    public:
        ~FVectorPath();
        FVectorPath();
        FVectorPath( std::string iName );
        FVectorSegment* AppendPoint( FVectorPoint* iPoint );
        virtual void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        virtual void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        bool PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) { return false; };
        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        FVectorPoint* GetLastPoint();
        FVectorSegment* GetLastSegment();
        std::list<FVectorPoint*>& GetSelectedPointList();
        virtual void PickPoint( double iX, double iY, double iRadius ) = 0;
        virtual void Unselect( FVectorPoint* iPoint ) = 0;
        void Clear();

};
