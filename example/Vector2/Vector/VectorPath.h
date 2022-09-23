#ifndef _FVECTORPATH_H_
#define _FVECTORPATH_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"

class FVectorPath : public FVectorObject
{
    private:
        FVectorPoint* mLastPoint;
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        bool PickShape( BLContext& iBLContext, double iX, double iY ) { return false; };

    protected :
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorSegment*> mSegmentList;
        std::list<FVectorPoint*> mSelectedPointList;
        void AddSegment( FVectorSegment* iSegment );
  
    public:
        ~FVectorPath();
        FVectorPath();
        FVectorSegment* AppendPoint(FVectorPoint* iPoint);
        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        FVectorPoint *GetLastPoint();
        void SetLastPoint( FVectorPoint* iLastPoint );
        std::list<FVectorPoint*> GetSelectedPointList();
        virtual void Pick( double iX, double iY, double iRadius ) = 0;
        virtual void Unselect( FVectorPoint* iPoint ) = 0;

};

#endif // _FVECTORPATH_H_