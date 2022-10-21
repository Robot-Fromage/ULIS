#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorPoint.h"
#include "Vector/VectorSegment.h"

class FVectorSegmentCubic : public FVectorSegment
{
    private:
        FVectorHandleSegment mCtrlPoint[2];

    public:
        ~FVectorSegmentCubic(){};
        FVectorSegmentCubic( );
        FVectorSegmentCubic( FVectorPoint* iPoint0, FVectorPoint* iPoint1 );
        FVectorSegmentCubic( FVectorPoint* iPoint0, double iCtrlPoint0x, double iCtrlPoint0y, double iCtrlPoint1x, double iCtrlPoint1y, FVectorPoint* iPoint1 );
        FVectorHandleSegment& GetControlPoint( int iCtrlPointNum );
        FVectorPointCubic& GetPoint( int iPointNum );
        void Draw( FBlock& iBlock, BLContext& iBLContext );
        void DrawStructure( FBlock& iBlock, BLContext& iBLContext );
        FVec2D GetPreviousVector(bool iNormalize);
        FVec2D GetNextVector(bool iNormalize);
        FVec2D GetVectorAtEnd( bool iNormalize );
        FVec2D GetVectorAtStart( bool iNormalize );
};
