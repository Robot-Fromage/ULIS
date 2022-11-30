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

        void DrawJoint( FVectorSegmentCubic* iPrevSegment
                      , FVectorSegmentCubic& iSegment
                      , double iRadius );

        uint32 mJointType;

    protected:
        FVectorObject* CopyShape();
        void DrawShape( FRectD& iRoi, uint64 iFlags );
        FVectorObject* PickShape( double iX, double iY, double iRadius );

    public:
        FVectorPathCubic();
        FVectorPathCubic( std::string iName );
        FVectorSegmentCubic* AppendPoint( FVectorPointCubic* iPoint, bool iConnect, bool iBuildSegments );

        bool PickPoint ( double iX, double iY, double iRadius, uint64 iSelectionFlags );
        void Unselect( FVectorPoint* iPoint );

        void DrawStructure( FRectD& iRoi );
        void setJointRadial();
        void setJointLinear();
        void setJointMiter();
        void setJointNone();
        void Fill( FRectD& iRoi );
        void Merge( FVectorPathCubic& iCubicPath );
        void DrawShapeVariable( FRectD& iRoi, uint64 iFlags );

        void Mirror( bool iMirrorX, bool iMirrorY );
        void Cut( FVec2D& linePoint0, FVec2D& linePoint1 );
};
