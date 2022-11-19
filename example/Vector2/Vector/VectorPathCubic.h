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
        FVectorPathCubic( std::string iName );
        FVectorSegmentCubic* AppendPoint( FVectorPointCubic* iPoint, bool iConnect, bool iBuildSegments );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );
        bool PickPoint ( double iX, double iY, double iRadius );
        void Unselect( FVectorPoint* iPoint );
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void setJointRadial();
        void setJointLinear();
        void setJointNone();
        void Fill( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void Merge( FVectorPathCubic& iCubicPath );
        void DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        FVectorObject* CopyShape();
};
