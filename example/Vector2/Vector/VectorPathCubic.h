#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"

class FVectorPathCubic: public FVectorPath
{
    private:
        static const uint32 JOINT_TYPE_RADIAL = 0;
        static const uint32 JOINT_TYPE_LINEAR = 1;
        static const uint32 JOINT_TYPE_NONE   = 2;

        void DrawSegmentVariableThickness( FBlock& iBlock
                                         , BLContext& iBLContext
                                         , FVectorSegmentCubic& iSegment
                                         , double iFromT
                                         , double iToT
                                         , FVec2D& iFromPoint // that way we dont have to query the cubic curve at point0 and point1
                                         , FVec2D& iToPoint // that way we dont have to query the cubic curve at point0 and point1
                                         , FVec2D* iPrevSegmentVector
                                         , FVec2D* iNextSegmentVector
                                         , double iStartRadius
                                         , double iEndRadius );
        void DrawSegmentVariable( FBlock& iBlock
                                , BLContext& iBLContext
                                , FVectorSegmentCubic& iSegment
                                , double iFromT
                                , double iToT
                                , double iStartRadius
                                , double iEndRadius
                                , FVec2D* iPrevSegmentVector
                                , FVec2D* iNextSegmentVector
                                , int32 iMaxRecurseDepth );
        void DrawJoint( FBlock& iBlock
                      , BLContext& iBLContext
                      , FVectorSegmentCubic* iPrevSegment
                      , FVectorSegmentCubic& iSegment
                      , double iRadius );
        void DrawSegment( BLPath& iPath, FVectorSegmentCubic& iSegment, bool iIsStandalone );
        void DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext );

        uint32 mJointType;
        
    public:
        FVectorPathCubic();
        FVectorSegmentCubic* AppendPoint( FVectorPoint* iPoint );
        bool PickShape ( double iX, double iY, double iRadius );
        void PickPoint ( double iX, double iY, double iRadius );
        void Unselect( FVectorPoint* iPoint );
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        void DrawStructure( FBlock& iBlock, BLContext& iBLContext );
        FVec2D GetPointPerpendicularVector( FVectorPoint& iPoint );
        void setJointRadial();
        void setJointLinear();
        void setJointNone();
};
