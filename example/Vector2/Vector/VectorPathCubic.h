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
        static const uint32 JOINT_TYPE_NONE   = 0;
        static const uint32 JOINT_TYPE_RADIAL = 1;
        static const uint32 JOINT_TYPE_LINEAR = 2;
        static const uint32 JOINT_TYPE_MITER  = 3;

        void DrawJoint( FBlock& iBlock
                      , BLContext& iBLContext
                      , FVectorSegmentCubic* iPrevSegment
                      , FVectorSegmentCubic& iSegment
                      , double iRadius );

        uint32 mJointType;

    protected:
        FVectorObject* CopyShape();
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );

    public:
        FVectorPathCubic();
        FVectorPathCubic( std::string iName );
        FVectorSegmentCubic* AppendPoint( FVectorPointCubic* iPoint, bool iConnect, bool iBuildSegments );

        bool PickPoint ( double iX, double iY, double iRadius, uint64 iSelectionFlags );
        void Unselect( FVectorPoint* iPoint );

        void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void setJointRadial();
        void setJointLinear();
        void setJointMiter();
        void setJointNone();
        void Fill( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void Merge( FVectorPathCubic& iCubicPath );
        void DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );

        void Mirror( bool iMirrorX, bool iMirrorY );
        void Cut( FVec2D& linePoint0, FVec2D& linePoint1 );
};
