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
    protected:
        FVectorHandleSegment mCtrlPoint[2];
        std::vector<FPolygon> mPolygonCache;
        double mDistanceSquared;
        BLPath mBLPath;

    private:
        void BuildVariableAdaptive( double  iFromT
                                  , double  iToT
                                  , double  iStartRadius
                                  , double  iEndRadius
                                  , FVec2D* iPrevSegmentVector
                                  , FVec2D* iNextSegmentVector
                                  , int32   iMaxRecurseDepth
                                  , int    *iPolygonID );

        void BuildVariableThickness( double iFromT
                                   , double iToT
                                   , FVec2D& iFromPoint
                                   , FVec2D& iToPoint
                                   , FVec2D* iPrevSegmentVector
                                   , FVec2D* iNextSegmentVector
                                   , double iStartRadius
                                   , double iEndRadius
                                   , int    iPolygonID );

    public:
        ~FVectorSegmentCubic(){};
        FVectorSegmentCubic( FVectorPathCubic& iPath, FVectorPointCubic* iPoint0, FVectorPointCubic* iPoint1 );
        FVectorSegmentCubic( FVectorPathCubic& iPath, FVectorPointCubic* iPoint0, double iCtrlPoint0x, double iCtrlPoint0y, double iCtrlPoint1x, double iCtrlPoint1y, FVectorPointCubic* iPoint1 );
        FVectorHandleSegment& GetControlPoint( int iCtrlPointNum );
        void Draw( FRectD &iRoi );
        void DrawStructure( FRectD &iRoi, double iZoomFactor );
        void DrawIntersections ( FRectD &iRoi, double iZoomFactor );
        FVec2D GetPreviousVector( bool iNormalize );
        FVec2D GetNextVector( bool iNormalize );
        FVec2D GetVectorAtEnd( bool iNormalize );
        FVec2D GetVectorAtStart( bool iNormalize );
        FRectD& GetBoundingBox();
        void UpdateBoundingBox();
        bool Pick( double iX, double iY, double iRadius );
        void IncreasePolygonCache(uint32 iSize);
        void ResetPolygonCache();
        uint32 GetPolygonCount(); // TODO: use vector size() method.
        std::vector<FPolygon>& GetPolygonCache();
        void IntersectPath( FVectorPathCubic& iPath );
        void Intersect( FVectorSegmentCubic& iOther );
        void Update();
        void BuildVariable();
        double GetDistanceSquared();
        FVec2D GetPointAt(double t);
        bool Cut( FVec2D& linePoint0, FVec2D& linePoint1 );
};