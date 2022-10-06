#ifndef _FVECTORPATHCUBIC_H_
#define _FVECTORPATHCUBIC_H_

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorPath.h"

class FVectorPathCubic: public FVectorPath
{
    private:
        void DrawSegment( BLPath& iPath, FVectorSegmentCubic& iSegment, FVec2D* iDrift0, FVec2D* iDrift1, bool iIsStandalone );
        void DrawShapeVariable( FBlock& iBlock, BLContext& iBLContext );

    public:
        FVectorPathCubic();
        FVectorSegmentCubic* AppendPoint( FVectorPoint* iPoint );
        bool PickShape ( double iX, double iY, double iRadius );
        void PickPoint ( double iX, double iY, double iRadius );
        void Unselect( FVectorPoint* iPoint );
        void DrawShape( FBlock& iBlock, BLContext& iBLContext );
        void DrawStructure( FBlock& iBlock, BLContext& iBLContext );
        FVec2D FVectorPathCubic::GetPointPerpendicularVector( FVectorPoint& iPoint );

};

#endif // _FVECTORPATHCUBIC_H_