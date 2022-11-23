#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorObject.h"
#include "Vector/VectorSegment.h"
#include "Vector/VectorLoop.h"

class FVectorPath : public FVectorObject
{
    private:
        void UpdateShape();
        virtual void DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );
        FVectorObject* PickShape( BLContext& iBLContext, double iX, double iY, double iRadius ) { return nullptr; };
        FVectorObject* CopyShape();

    protected :
        std::list<FVectorLoop*> mLoopList; // list of loops
        std::list<FVectorPoint*> mPointList;
        std::list<FVectorSegment*> mSegmentList;
        std::list<FVectorSegment*> mInvalidatedSegmentList;
        std::list<FVectorLoop*> mInvalidatedLoopList;
        std::list<FVectorPoint*> mSelectedPointList;
        BLPath mPath;

    public:
        static const uint64 PICK_HANDLE_POINT   = 1;
        static const uint64 PICK_HANDLE_SEGMENT = 1 << 1;
        static const uint64 PICK_POINT          = 1 << 2;

        ~FVectorPath();
        FVectorPath();
        FVectorPath( std::string iName );
        void AddSegment(FVectorSegment* iSegment);
        void RemoveSegment(FVectorSegment* iSegment);
        void AddPoint( FVectorPoint* iPoint );
        virtual FVectorSegment* AppendPoint( FVectorPoint* iPoint, FVectorPoint* iPreviousPoint );
        FVectorObject* PickLoops(BLContext& iBLContext,double iX,double iY,double iRadius);
        void DrawLoops( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );

        virtual void DrawStructure( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi );

        /*virtual void InsertPoint( FVectorSegment* iSegment, FVectorPoint* iPoint );*/
        std::list<FVectorSegment*>& GetSegmentList();
        FVectorPoint* GetFirstPoint();
        FVectorPoint* GetLastPoint();
        FVectorSegment* GetFirstSegment();
        FVectorSegment* GetLastSegment();
        std::list<FVectorPoint*>& GetSelectedPointList();
        virtual bool PickPoint( double iX, double iY, double iRadius, uint64 iSelectionFlags ) = 0;
        virtual void Unselect( FVectorPoint* iPoint ) = 0;
        void Clear();
        bool IsLoop();
        FVectorLoop* GetLoopByID( uint64 iID );
        void AddLoop( FVectorLoop* iLoop );
        void RemoveLoop( FVectorLoop* iLoop );

        void InvalidateSegment(FVectorSegment* iSegment);
        void InvalidateLoop( FVectorLoop* iLoop );
        void UpdateBBox();

};
