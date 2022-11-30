#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPoint::~FVectorPoint()
{
}

FVectorPoint::FVectorPoint()
{
    Set( 0.0f, 0.0f );
}

FVectorPoint::FVectorPoint( double iX, double iY )
{
    Set( iX, iY );
}

FVec2D&
FVectorPoint::GetCoords()
{
    return mCoords;
}

double 
FVectorPoint::GetX()
{
    return mCoords.x;
}

double 
FVectorPoint::GetY()
{
    return mCoords.y;
}

void
FVectorPoint::InvalidateSegments()
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        segment->Invalidate();
    }
}

void 
FVectorPoint::SetX( double iX )
{
    mCoords.x  = iX;

    InvalidateSegments();
}

void 
FVectorPoint::SetY( double iY )
{
    mCoords.y = iY;

    InvalidateSegments();
}

void 
FVectorPoint::Set( double iX, double iY )
{
    mCoords.x = iX;
    mCoords.y = iY;

    InvalidateSegments();
}

void
FVectorPoint::AddSegment( FVectorSegment* iSegment )
{
    mSegmentList.push_back( iSegment );
}

void
FVectorPoint::RemoveSegment( FVectorSegment* iSegment )
{
    mSegmentList.remove( iSegment );
}

uint32
FVectorPoint::GetSegmentCount( )
{
    return mSegmentList.size();
}

std::list<FVectorSegment*>&
FVectorPoint::GetSegmentList()
{
    return mSegmentList;
}

uint32
FVectorPoint::GetType()
{
    return POINT_TYPE_REGULAR; // default type;
}

double
FVectorPoint::GetRadius()
{
    return mRadius;
}

void
FVectorPoint::SetRadius( double iRadius
                       , bool iBuildSegments )
{
    mRadius = iRadius;
}

FVectorSegment*
FVectorPoint::GetLastSegment()
{
    return mSegmentList.back();
}

FVectorSegment*
FVectorPoint::GetFirstSegment()
{
    return mSegmentList.front();
}

static uintptr_t
GenerateSegmentID( FVectorSegment* iSegment, FVectorPoint* iP0, FVectorPoint* iP1 )
{
    uintptr_t xorPoint = reinterpret_cast<uintptr_t>(iP0) ^ reinterpret_cast<uintptr_t>(iP1);

    return xorPoint ^ reinterpret_cast<uintptr_t>(iSegment);
}

static bool seekPoint( std::list<FVectorPoint*>& iPointList
                     , FVectorPoint* iPoint )
{
    for( std::list<FVectorPoint*>::iterator it = iPointList.begin(); it != iPointList.end(); ++it )
    {
        if ( static_cast<FVectorPoint*>(*it) == iPoint )
        {
            return true;
        }
    }

    return false;
}

double
FVectorPoint::GetT( FVectorSegment& iSegment )
{
    return ( iSegment.GetPoint(0) == this ) ? 0.0f : 1.0f;
}

static bool seekSection( std::list<FVectorSection*>& iSectionList
                       , FVectorSection *iSection )
{
    for( std::list<FVectorSection*>::iterator it = iSectionList.begin(); it != iSectionList.end(); ++it )
    {
        if ( static_cast<FVectorSection*>(*it) == iSection )
        {
            return true;
        }
    }

    return false;
}

void
FVectorPoint::AddSection( FVectorSection* iSection )
{
    mSectionList.push_back( iSection );
}

void
FVectorPoint::RemoveSection( FVectorSection* iSection )
{
    mSectionList.remove( iSection );
}

bool
FVectorPoint::HasSegment( FVectorSegment& iSegment )
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment == &iSegment )
        {
            return true;
        }
    }

    return false;
}

void
FVectorPoint::AddLoop( FVectorLoop* iLoop )
{
    mLoopList.push_back( iLoop );
}

void
FVectorPoint::RemoveLoop( FVectorLoop* iLoop )
{
    mLoopList.remove( iLoop );
}

void
FVectorPoint::InvalidateLoops()
{
    for( std::list<FVectorLoop*>::iterator it = mLoopList.begin(); it != mLoopList.end(); ++it )
    {
        FVectorLoop* loop = static_cast<FVectorLoop*>(*it);

        loop->Invalidate();
    }
}

bool
FVectorPoint::IsClosestSection( FVectorSection& iStartSection
                              , FVectorSection& iEndSection )
{
    double startSectionP0T   = iStartSection.GetPoint( 0 )->GetT( iStartSection.GetSegment() );
    double startSectionP1T   = iStartSection.GetPoint( 1 )->GetT( iStartSection.GetSegment() );
    double startSectionT     = ( startSectionP0T + startSectionP1T ) * 0.5f;
    FVec2D startSectionMidAt = iStartSection.GetSegment().GetPointAt( startSectionT );

    double endSectionP0T     = iEndSection.GetPoint( 0 )->GetT( iEndSection.GetSegment() );
    double endSectionP1T     = iEndSection.GetPoint( 1 )->GetT( iEndSection.GetSegment() );
    double endSectionT       = ( endSectionP0T + endSectionP1T ) * 0.5f;
    FVec2D endSectionMidAt   = iEndSection.GetSegment().GetPointAt( endSectionT );

    FVec3F originToStartMid = { startSectionMidAt.x - this->GetCoords().x
                              , startSectionMidAt.y - this->GetCoords().y, 0.0f };
    FVec3F originToEndMid   = { endSectionMidAt.x - this->GetCoords().x
                              , endSectionMidAt.y - this->GetCoords().y, 0.0f };

    if ( originToStartMid.DistanceSquared() && originToEndMid.DistanceSquared() )
    {
        originToStartMid.Normalize();
        originToEndMid.Normalize();

        FVec3F referenceCross = originToStartMid.CrossProduct( originToEndMid );

        for( std::list<FVectorSection*>::iterator it = mSectionList.begin(); it != mSectionList.end(); ++it )
        {
            FVectorSection* section = static_cast<FVectorSection*>(*it);

            if ( ( section != &iStartSection ) && ( section != &iEndSection ) )
            {
                double p0T = section->GetPoint(0)->GetT(section->GetSegment());
                double p1T = section->GetPoint(1)->GetT(section->GetSegment());
                double t = ( p0T + p1T ) * 0.5f;
                FVec2D pointAt = section->GetSegment().GetPointAt( t );
                FVec3F originToPoint = { pointAt.x - this->GetCoords().x
                                       , pointAt.y - this->GetCoords().y, 0.0f };

               if ( originToPoint.DistanceSquared() )
               {
                    originToPoint.Normalize();

                    FVec3F sectionCross = originToStartMid.CrossProduct( originToPoint );

                    // same side
                    if ( sectionCross.DotProduct( referenceCross ) > 0.0f )
                    {
                        if ( originToStartMid.DotProduct( originToPoint ) > originToStartMid.DotProduct( originToEndMid ) )
                        {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

FVec2D
FVectorPoint::GetPosition( FVectorSegment& iSegment )
{
    return mCoords;
}

std::list<FVectorSection*>&
FVectorPoint::GetSectionList()
{
    return mSectionList;
}

typedef struct _TREENODE
{
    struct _TREENODE* mParent;
    FVectorPoint* mPoint;
    FVectorSection* mSection;
    _TREENODE( struct _TREENODE* iParent, FVectorPoint* iPoint, FVectorSection* iSection )
        : mParent ( iParent )
        , mPoint ( iPoint )
        , mSection ( iSection ) {};
} TREENODE;

void TREENODETOLIST( TREENODE* node
                   , std::list<FVectorPoint*>& iPointList
                   , std::list<FVectorSection*>& iSectionList )
{
    while ( node->mSection )
    {
        iPointList.push_back ( node->mPoint );
        iSectionList.push_back ( node->mSection );

        node = node->mParent;
    }
}

bool TREENODESEEKSECTION( TREENODE* node, FVectorSection* section )
{
    do
    {
        if ( section == node->mSection )
        {
            return true;
        }
    } while ( node = node->mParent );

    return false;
}

void
FVectorPoint::March()
{
    static int depth; // inits to zero by standard
    FVectorPoint* sourcePoint = this;
    uint32 maxLoopcount = sourcePoint->GetSectionList().size();
    TREENODE *node = new TREENODE ( NULL, this, NULL );
    std::list<TREENODE*> nodeList;

    depth++;

    nodeList.push_back ( node );

    while ( nodeList.size() )
    {
        TREENODE *parentNode = nodeList.back();

        std::list<FVectorSection*>& sectionList = parentNode->mPoint->GetSectionList();

        for( std::list<FVectorSection*>::iterator it = sectionList.begin(); it != sectionList.end(); ++it )
        {
            FVectorSection* section = static_cast<FVectorSection*>(*it);

            if ( /*section != parentNode->mSection*/ TREENODESEEKSECTION( parentNode, section ) == false )
            {
                FVectorPoint* nextPoint = ( parentNode->mPoint == section->GetPoint(0) ) ? section->GetPoint(1) : section->GetPoint(0);
                TREENODE* nextNode = new TREENODE ( parentNode, nextPoint, section );

                if ( nextPoint == this )
                {
                    FVectorPath& path = nextNode->mSection->GetSegment().GetPath();
                    std::list<FVectorPoint*> loopPointList;
                    std::list<FVectorSection*> loopSectionList;
                    uint64 loopID;

                    TREENODETOLIST ( nextNode, loopPointList, loopSectionList );
printf("loop detected\n");
                    loopID = FVectorLoop::GenerateID( loopSectionList );

                    if ( path.GetLoopByID( loopID ) == nullptr )
                    {
                        if ( IsClosestSection ( *loopSectionList.front(), *loopSectionList.back() ) == true )
                        {
                            FVectorLoop *loop = new FVectorLoop ( path, loopID, *nextNode->mPoint, loopPointList, loopSectionList );

                            path.AddLoop( loop );
                        }
                    }
                    else
                    {
printf("path already exists\n");
                    }

                    delete nextNode;
                }
                else
                {
                    nodeList.push_back( nextNode );
                }
            }
        }

        nodeList.remove( parentNode );
    }

    depth--;
}

#ifdef ORIGINAL_VERSION
void
FVectorPoint::March( FVectorPointIntersection& iInitiatorPoint
                   , FVectorSegment& iCurrentSegment )
{
    static std::list<FVectorPoint*> loopPointList;
    static std::list<uintptr_t> iSegmentIDList;

    loopPointList.push_back ( this );

    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);
        double t =  static_cast<FVectorPointIntersection*>(this)->GetT(*segment);
        FVectorPoint* forwardPoint = segment->GetNextPoint(t);
        FVectorPoint* backwardPoint = segment->GetPreviousPoint(t);

        if ( forwardPoint )
        {
            if ( seekPoint( loopPointList, forwardPoint ) == false )
            {
                iSegmentIDList.push_back( GenerateSegmentID( segment, this, forwardPoint) );
                forwardPoint->March( iInitiatorPoint, *segment );
                iSegmentIDList.pop_back();
            }
            else
            {
                if ( seekSegment( iSegmentIDList, GenerateSegmentID( segment, this, forwardPoint ) ) == false )
                {
                    uint64 loopID = FVectorPathLoop::GenerateID( loopPointList, forwardPoint );

                    if ( segment->GetPath().GetPathLoopByID( loopID ) == nullptr )
                    {
                        FVectorPathLoop *loopPath = new FVectorPathLoop ( segment->GetPath(), loopID, loopPointList, forwardPoint );

                        segment->GetPath().AddLoop( loopPath );
                    } else printf("loop exists!!\n");
                }
            }
        }

        if( backwardPoint )
        {
            if( seekPoint( loopPointList, backwardPoint ) == false )
            {
                iSegmentIDList.push_back( GenerateSegmentID( segment, this, backwardPoint ) );
                backwardPoint->March( iInitiatorPoint, *segment );
                iSegmentIDList.pop_back();
            }
            else
            {
                if ( seekSegment( iSegmentIDList, GenerateSegmentID( segment, this, backwardPoint ) ) == false )
                {
                    uint64 loopID = FVectorPathLoop::GenerateID( loopPointList, backwardPoint );

                    if ( segment->GetPath().GetPathLoopByID( loopID ) == nullptr )
                    {
                        FVectorPathLoop *loopPath = new FVectorPathLoop ( segment->GetPath(), loopID, loopPointList, backwardPoint );

                        segment->GetPath().AddLoop( loopPath );
                    } else printf("loop exists!!\n");
                }
            }
        }
    }

    loopPointList.pop_back ( );
}
#endif

FVectorSegment*
FVectorPoint::GetSegment( FVectorPoint& iOtherPoint )
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( ( ( segment->GetPoint(0) == this ) && ( segment->GetPoint(1) == &iOtherPoint ) ) 
          || ( ( segment->GetPoint(1) == this ) && ( segment->GetPoint(0) == &iOtherPoint ) ) )
        {
            return segment;
        }
    }

    return nullptr;
}

FVectorSegment*
FVectorPoint::GetOtherSegment( FVectorSegment& iCurrentSegment )
{
    for( std::list<FVectorSegment*>::iterator it = mSegmentList.begin(); it != mSegmentList.end(); ++it )
    {
        FVectorSegment* segment = static_cast<FVectorSegment*>(*it);

        if ( segment != &iCurrentSegment )
        {
            return segment;
        }
    }

    return nullptr;
}
