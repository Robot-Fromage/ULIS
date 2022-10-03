#ifndef _FVECTORPATH_H_
#define _FVECTORPATH_H_

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

    public:
        ~FVectorPath();
        FVectorPath();
        FVectorSegment* AppendPoint(FVectorPoint* iPoint);
        virtual void DrawShape(FBlock& iBlock,BLContext& iBLContext);
        virtual void DrawStructure(FBlock& iBlock,BLContext& iBLContext);
        bool PickShape(BLContext& iBLContext,double iX,double iY) { return false; };
        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        FVectorPoint* GetLastPoint();
        FVectorSegment* GetLastSegment();
        std::list<FVectorPoint*>& GetSelectedPointList();
        virtual void Pick( double iX, double iY, double iRadius ) = 0;
        virtual void Unselect( FVectorPoint* iPoint ) = 0;
        void Clear();

};

#endif // _FVECTORPATH_H_