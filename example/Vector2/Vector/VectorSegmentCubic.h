#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"
#include "Vector/VectorSegment.h"

class FVectorPathCubic;

typedef struct _FPolygon {
    FVec2D quadVertex[4];
    FVec2D lineVertex[2];
    double fromT;
    double toT;
} FPolygon;

class FVectorSegmentCubic : public FVectorSegment
{
    private:
        FVectorHandleSegment mCtrlPoint[2];
        std::list<FVectorPointIntersection*> mIntersectionPointList;
        std::vector<FPolygon> mPolygonCache;
        uint32 mPolygonSlot;
        FRectD mBBox;

        void BuildVariableAdaptive( double iFromT
                                  , double iToT
                                  , double iStartRadius
                                  , double iEndRadius
                                  , FVec2D* iPrevSegmentVector
                                  , FVec2D* iNextSegmentVector
                                  , int32 iMaxRecurseDepth );

        void BuildVariableThickness( double iFromT
                                   , double iToT
                                   , FVec2D& iFromPoint
                                   , FVec2D& iToPoint
                                   , FVec2D* iPrevSegmentVector
                                   , FVec2D* iNextSegmentVector
                                   , double iStartRadius
                                   , double iEndRadius );

    public:
        ~FVectorSegmentCubic(){};
        FVectorSegmentCubic( );
        FVectorSegmentCubic( FVectorPointCubic* iPoint0, FVectorPointCubic* iPoint1 );
        FVectorSegmentCubic( FVectorPointCubic* iPoint0, double iCtrlPoint0x, double iCtrlPoint0y, double iCtrlPoint1x, double iCtrlPoint1y, FVectorPointCubic* iPoint1 );
        FVectorHandleSegment& GetControlPoint( int iCtrlPointNum );
        FVectorPointCubic& GetPoint( int iPointNum );
        void Draw( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi, double iZoomFactor );
        void DrawIntersections ( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi, double iZoomFactor );
        FVec2D GetPreviousVector(bool iNormalize);
        FVec2D GetNextVector(bool iNormalize);
        FVec2D GetVectorAtEnd( bool iNormalize );
        FVec2D GetVectorAtStart( bool iNormalize );
        FRectD& GetBoundingBox();
        void UpdateBoundingBox();

        bool Pick( double iX, double iY, double iRadius );
        void IncreasePolygonCache(uint32 iSize);
        void ResetPolygonCache();
        FPolygon* GetPolygonCacheSlot();

        void IntersectPath( FVectorPathCubic& iPath );
        void Intersect( FVectorSegmentCubic& iOther );

        void BuildVariable();
};
