#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>

class FVectorPoint;
class FVectorPath;
class FVectorPointIntersection;
class FVectorSection;

class FVectorSegment
{
    protected:
        FVectorPoint* mPoint[2];
        std::list<FVectorPointIntersection*> mIntersectionPointList;
        std::list<FVectorSection*> mSectionList;
        FVectorPath& mPath;
        FRectD mBBox;

    public:
        ~FVectorSegment();
        FVectorSegment( FVectorPath& iPath, FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorPoint* GetPoint( int iPointNum );
        virtual void Draw( FRectD &iRoi );
        virtual void DrawStructure( FRectD &iRoi );
        double GetStraightDistance();
        FVectorSegment* GetPreviousSegment();
        FVectorSegment* GetNextSegment();
        std::list<FVectorPointIntersection*>& GetIntersectionPointList();
        bool HasIntersectionPoint( FVectorPointIntersection& mIntersectionPoint );
        FVectorPoint* GetNextPoint( double iT );
        FVectorPoint* GetPreviousPoint( double iT ); 
        FVectorPath& GetPath();
        virtual void Update() {};
        void Invalidate();
        virtual double GetDistanceSquared();

        virtual FVec2D GetPointAt( double t );

        void ClearIntersections();
        FVectorSection* GetSection (double t);

        void AddIntersection ( FVectorPointIntersection* iIntersectionPoint );
        void RemoveSection ( FVectorSection* iSection );
        void AddSection ( FVectorSection* iSection );
        FVec2D GetVector( bool iNormalize );

        virtual FRectD& GetBoundingBox() { return mBBox; };

        FVectorSegment* GetOtherSegment( FVectorSegment& iCurrentSegment );
};
