#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"
#include "Vector/VectorLink.h"

class FVectorPath;
class FVectorPointIntersection;
class FVectorSection;

class FVectorSegment : public FVectorLink
{
    protected:
        std::list<FVectorPointIntersection*> mIntersectionPointList;
        std::list<FVectorSection*> mSectionList;
        FVectorPath& mPath;
        FRectD mBBox;

    public:
        ~FVectorSegment();
        FVectorSegment( FVectorPath& iPath, FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        virtual void Draw( FRectD &iRoi );
        virtual void DrawStructure( FRectD &iRoi );

        FVectorSegment* GetPreviousSegment();
        FVectorSegment* GetNextSegment();
        std::list<FVectorPointIntersection*>& GetIntersectionPointList();
        bool HasIntersectionPoint( FVectorPointIntersection& mIntersectionPoint );
        FVectorPoint* GetNextPoint( double iT );
        FVectorPoint* GetPreviousPoint( double iT ); 
        FVectorPath& GetPath();
        virtual void Update() {};
        void Invalidate();

        void ClearIntersections();
        FVectorSection* GetSection (double t);

        void AddIntersection ( FVectorPointIntersection* iIntersectionPoint );
        void RemoveSection ( FVectorSection* iSection );
        void AddSection ( FVectorSection* iSection );

        virtual FRectD& GetBoundingBox() { return mBBox; };

        FVectorSegment* GetOtherSegment( FVectorSegment& iCurrentSegment );
};
