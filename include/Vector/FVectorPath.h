#ifndef _FVECTORPATH_H_
#define _FVECTORPATH_H_

#include <ULIS>
#include <blend2d.h>

using namespace ::ULIS;

#include "Vector/FVectorObject.h"
#include "FVectorPoint.h"
#include "FVectorSegment.h"

class ULIS_API FVectorPath : public FVectorObject
{
    private:
        FVectorPoint* mLastPoint;

    protected :
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorSegment*> mSegmentList;
        std::list<FVectorPoint*> mSelectedPointList;
        void AddSegment( FVectorSegment* iSegment );
  
    public:
        ~FVectorPath();
        FVectorPath();
        virtual FVectorSegment* AppendPoint(FVectorPoint* iPoint);
        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        FVectorPoint *GetLastPoint();
        void SetLastPoint( FVectorPoint* iLastPoint );
        std::list<FVectorPoint*> GetSelectedPointList();
        virtual void Pick( double iX, double iY, double iRadius ) = 0;
        virtual void Unselect( FVectorPoint* iPoint ) = 0;
        void Draw( FBlock& iBlock, BLContext& iBLContext );
};

#endif // _FVECTORPATH_H_