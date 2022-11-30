#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorPath.h"

class FVectorLoop : public FVectorObject
{
    private:
        void UpdateShape();
        FVectorObject* CopyShape();

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

        void DrawShape( FRectD& iRoi, uint64 iFlags );
        FVectorObject* PickShape( double iX, double iY, double iRadius );
        void DrawPoints( FRectD& iRoi );

        void Unselect( FVectorPoint* iPoint ) { };
        bool PickPoint( double iX, double iY, double iRadius ) { return false; };
        uint64 GetID();
        static uint64 GenerateID( std::list<FVectorSection*> iSectionList );
        void Build();
        void Invalidate();

        void Attach();
        void Detach();

};