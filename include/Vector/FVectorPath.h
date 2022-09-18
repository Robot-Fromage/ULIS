#ifndef _FVECTORPATH_H_
#define _FVECTORPATH_H_

#include <ULIS>
#include <blend2d.h>

#include "Vector/FVectorObject.h"
#include "Vector/FVectorPoint.h"
#include "Vector/FVectorSegment.h"

ULIS_NAMESPACE_BEGIN

class ULIS_API FVectorPath : public FVectorObject
{
    private:
        FVectorPoint* mLastPoint;
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );

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

};

ULIS_NAMESPACE_END

#endif // _FVECTORPATH_H_