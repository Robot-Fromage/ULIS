#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorPath.h"

class FVectorPathLoop : public FVectorPath
{
    private:
        void BuildSegmentCubic( std::vector<BLPoint>& iPointArray
                              , FVectorSegmentCubic& iSegment
                              , double iFromT
                              , double iToT );

    protected :
        uint64 mID;
        std::vector<BLPoint> mPointArray;
        BLPath mPath;

    public:
        ~FVectorPathLoop();
        FVectorPathLoop( FVectorPath& iParent, uint64 iID, std::list<FVectorPoint*>& iLoopPointist, FVectorPoint* iLoopPoint );
        void ShapeFromPointList( std::list<FVectorPoint*> iLoopPointist, FVectorPoint* iLoopPoint );
        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void DrawPoints( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );
        void Unselect( FVectorPoint* iPoint ) { };
        bool PickPoint( double iX, double iY, double iRadius ) { return false; };
        uint64 GetID();
        static uint64 GenerateID( std::list<FVectorPoint*> iLoopPointist, FVectorPoint* iLoopPoint );
        void Build();
        void Invalidate();
        void UpdateShape();

};
