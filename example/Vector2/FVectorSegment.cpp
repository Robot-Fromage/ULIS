#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSegment::~FVectorSegment()
{
}

FVectorSegment::FVectorSegment( FVectorPath& iPath
                              , FVectorPoint* iPoint0
                              , FVectorPoint* iPoint1 )
    : mPath ( iPath )
{
    mPoint[0] = iPoint0;
    mPoint[1] = iPoint1;

    AddSection ( new FVectorSection ( *this, mPoint[0], mPoint[1] ) );
}

FVectorPoint*
FVectorSegment::GetPoint( int iPointNum )
{
    return mPoint[iPointNum];
}

FVectorPath&
FVectorSegment::GetPath()
{
    return mPath;
}

FVec2D
FVectorSegment::GetVector( bool iNormalize )
{
    FVec2D vec = mPoint[1]->GetCoords() - mPoint[0]->GetCoords();

    if ( iNormalize == true )
    {
        if ( vec.DistanceSquared() )
        {
            vec.Normalize();
        }
    }

    return vec;
}

double
FVectorSegment::GetDistanceSquared()
{
    FVec2D dist = mPoint[1]->GetCoords() - mPoint[0]->GetCoords();

    return dist.DistanceSquared();
}

FVectorSection*
FVectorSegment::GetSection ( double t )
{
    for( std::list<FVectorSection*>::iterator it = mSectionList.begin(); it != mSectionList.end(); ++it )
    {
        FVectorSection* section = static_cast<FVectorSection*>(*it);
        FVectorPoint* point0 = section->GetPoint(0);
        FVectorPoint* point1 = section->GetPoint(1);
        double fromT = point0->GetT( *this );
        double   toT = point1->GetT( *this );

        if ( ( t >= fromT ) && ( t <= toT ) )
        {
            return section;
        }
    }

    return nullptr;
}

FVec2D
FVectorSegment::GetPointAt ( double t )
{
    FVec2D& p0 =  mPoint[0]->GetCoords();
    FVec2D& p1 =  mPoint[0]->GetCoords();
    FVec2D vec = p1 - p0;

    return p0 + ( vec * t );
}

void
FVectorSegment::AddSection ( FVectorSection* iSection )
{
    iSection->GetPoint(0)->AddSection( iSection );
    iSection->GetPoint(1)->AddSection( iSection );

    mSectionList.push_back( iSection );
}

void
FVectorSegment::RemoveSection ( FVectorSection* iSection )
{
    std::list<FVectorLoop*> loopList = iSection->GetLoopList();

    iSection->GetPoint(0)->RemoveSection( iSection );
    iSection->GetPoint(1)->RemoveSection( iSection );

    mSectionList.remove( iSection );

    for( std::list<FVectorLoop*>::iterator it = loopList.begin(); it != loopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

        mPath.RemoveLoop( loop );
    }
}

void
FVectorSegment::AddIntersection ( FVectorPointIntersection* iIntersectionPoint )
{
    double t = iIntersectionPoint->GetT( *this );
    FVectorSection* section = GetSection ( t );
    FVectorPoint* point[3] = { section->GetPoint(0), iIntersectionPoint, section->GetPoint(1) };
    FVectorSection* subSection[2] = { new FVectorSection ( *this, point[0], point[1] )
                                    , new FVectorSection ( *this, point[1], point[2] ) };

    // this does not release memory so that we can undo that later
    RemoveSection( section );

    mIntersectionPointList.push_back( iIntersectionPoint );

    AddSection ( subSection[0] );
    AddSection ( subSection[1] );
}

void
FVectorSegment::ClearIntersections ( )
{
    for( std::list<FVectorSection*>::iterator it = mSectionList.begin(); it != mSectionList.end(); ++it )
    {
        FVectorSection* section = static_cast<FVectorSection*>(*it);
        FVectorPoint* point0 = section->GetPoint(0);
        FVectorPoint* point1 = section->GetPoint(1);

        point0->RemoveSection( section );
        point1->RemoveSection( section );
    }

    // We do not release memory so that we can undo that later
    mSectionList.clear();

    // We do not release memory so that we can undo that later
    mIntersectionPointList.clear();

    // Add default section
    AddSection ( new FVectorSection ( *this, mPoint[0], mPoint[1] ) );
}

void
FVectorSegment::Invalidate()
{
    for( std::list<FVectorPointIntersection*>::iterator it = mIntersectionPointList.begin(); it != mIntersectionPointList.end(); ++it )
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*it);

        intersectionPoint->InvalidateLoops();
    }

    mPath.InvalidateSegment( this );
    mPath.Invalidate();
}

double
FVectorSegment::GetStraightDistance()
{
    FVec2D vec = { mPoint[1]->GetX() - mPoint[0]->GetX(),
                   mPoint[1]->GetY() - mPoint[0]->GetY() };

    return vec.Distance();
}

void
FVectorSegment::DrawStructure( FBlock& iBlock
                             , BLContext& iBLContext
                             , FRectD &iRoi )
{

}

std::list<FVectorPointIntersection*>&
FVectorSegment::GetIntersectionPointList()
{
    return mIntersectionPointList;
}

bool
FVectorSegment::HasIntersectionPoint( FVectorPointIntersection& mIntersectionPoint )
{
    for( std::list<FVectorPointIntersection*>::iterator it = mIntersectionPointList.begin(); it != mIntersectionPointList.end(); ++it )
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*it);

        if ( intersectionPoint == &mIntersectionPoint ) 
        {
            return true;
        }
    }

    return false;
}

FVectorPoint*
FVectorSegment::GetNextPoint( double iT )
{
    FVectorPoint* closestPoint = nullptr;
    double closestT = 1.0f;

    // Search intersection point between this intersection point and the next segment point
    for(std::list<FVectorPointIntersection*>::iterator it = mIntersectionPointList.begin(); it != mIntersectionPointList.end(); ++it)
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*it);
        double pointT = intersectionPoint->GetT(*this);

        if( pointT > iT )
        {
            if ( pointT <= closestT )
            {
                closestPoint = intersectionPoint;

                closestT = pointT;
            }
        }
    }

    return ( closestPoint ) ? closestPoint : mPoint[1];
}

FVectorPoint*
FVectorSegment::GetPreviousPoint( double iT )
{
    FVectorPoint* closestPoint = nullptr;
    double closestT = 0.0f;

    // Search intersection point between this intersection point and the next segment point
    for(std::list<FVectorPointIntersection*>::iterator iter = mIntersectionPointList.begin(); iter != mIntersectionPointList.end(); ++iter)
    {
        FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(*iter);
        double pointT = intersectionPoint->GetT(*this);

        if( pointT < iT )
        {
            if ( pointT >= closestT )
            {
                closestPoint = intersectionPoint;

                closestT = pointT;
            }
        }
    }

    return ( closestPoint ) ? closestPoint : mPoint[0];
}

FVectorSegment*
FVectorSegment::GetNextSegment( )
{
    std::list<FVectorSegment*>& segmentList = mPoint[1]->GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != this ) 
        {
            return segment;
        }
    }

    return nullptr;
}

FVectorSegment* FVectorSegment::GetPreviousSegment( )
{
    std::list<FVectorSegment*>& segmentList = mPoint[0]->GetSegmentList();

    for( std::list<FVectorSegment*>::iterator it = segmentList.begin(); it != segmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != this ) 
        {
            return segment;
        }
    }

    return nullptr;
}

void
FVectorSegment::Draw( FBlock& iBlock
                    , BLContext& iBLContext
                    , FRectD &iRoi )
{

}
