#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorPath.h"

class FVectorLoop : public FVectorObject
{
    private:
        void BuildSegmentCubic( std::vector<BLPoint>& iPointArray
                              , FVectorSegmentCubic& iSegment
                              , double iFromT
                              , double iToT );

    protected :
        uint64 mID;
        FVectorPoint& mLoopPoint;
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorSection*> mSectionList;

        std::vector<BLPoint> mPointArray;
        BLPath mPath;

    public:
        ~FVectorLoop();
        FVectorLoop::FVectorLoop( FVectorPath& iParent
                                , uint64 iID
                                , FVectorPoint& iLoopPoint
                                , std::list<FVectorPoint*>& iPointList
                                , std::list<FVectorSection*>& iSectionList );

        void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        void DrawPoints( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius );
        void Unselect( FVectorPoint* iPoint ) { };
        bool PickPoint( double iX, double iY, double iRadius ) { return false; };
        uint64 GetID();
        static uint64 GenerateID( std::list<FVectorSection*> iSectionList );
        void Build();
        void Invalidate();
        void UpdateShape();

        void Attach();
        void Detach();
};
