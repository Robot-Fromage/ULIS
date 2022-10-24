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

        void DrawJoint( FBlock& iBlock
                      , BLContext& iBLContext
                      , FVectorSegmentCubic* iPrevSegment
                      , FVectorSegmentCubic& iSegment
                      , double iRadius );

        uint32 mJointType;
        
    public:
        FVectorPathCubic();
        FVectorSegmentCubic* AppendPoint( FVectorPointCubic* iPoint, bool iBuildSegments );
        bool PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );
        void PickPoint ( double iX, double iY, double iRadius );
        void Unselect( FVectorPoint* iPoint );
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void setJointRadial();
        void setJointLinear();
        void setJointNone();

        void DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
};
