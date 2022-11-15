#pragma once

#include <blend2d.h>
#include <Core/Core.h>
#include <Image/Block.h>
#include "Vector/VectorSegment.h"

class FVectorPathLoop;
class FVectorSection;

class FVectorPoint
{
    private:

    protected:
        FVec2D mCoords;
        std::list<FVectorSegment*> mSegmentList;
        std::list<FVectorSection*> mSectionList;
        double mRadius;

    public:
        static  const uint32 POINT_TYPE_REGULAR        = 0;
        static  const uint32 POINT_TYPE_INTERSECTION   = 1;
        static  const uint32 POINT_TYPE_HANDLE_POINT   = 2;
        static  const uint32 POINT_TYPE_HANDLE_SEGMENT = 3;

        ~FVectorPoint();
        FVectorPoint();
        FVectorPoint( double iX, double iY );
        virtual void AddSegment( FVectorSegment* iSegment );
        void AddSection( FVectorSection* iSection );
        void RemoveSection( FVectorSection* iSection );
        FVectorSegment* GetLastSegment();
        FVectorSegment* GetFirstSegment();
        virtual FVectorSegment* GetSegment( FVectorPoint& iOtherPoint );
        void RemoveSegment( FVectorSegment* iSegment );
        FVec2D& GetCoords();
        double GetX();
        double GetY();
        virtual void SetX( double iX );
        virtual void SetY( double iY );
        virtual void Set( double iX, double iY );
        uint32 GetSegmentCount();
        std::list<FVectorSegment*>& GetSegmentList();
        virtual uint32 GetType();
        virtual double GetT( FVectorSegment& );
        double GetRadius();
        virtual void SetRadius( double iRadius, bool iBuildSegments );
        void InvalidateSegments();
        void March( FVectorPointIntersection&  iInitiatorPoint
                   , FVectorSegment&       iCurrentSegment );
};
