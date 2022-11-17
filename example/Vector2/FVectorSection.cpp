#include <blend2d.h>
#include "Vector/Vector.h"

FVectorSection::~FVectorSection()
{
}

FVectorSection::FVectorSection( FVectorSegment& iSegment
                              , FVectorPoint* iPoint0
                              , FVectorPoint* iPoint1 )
    : mSegment ( iSegment )
    , mPoint { iPoint0, iPoint1 }
{

}

FVectorSegment&
FVectorSection::GetSegment()
{
    return mSegment;
}

FVectorPoint*
FVectorSection::GetPoint( int iNum )
{
    return mPoint[iNum];
}

void
FVectorSection::AddLoop( FVectorLoop* iLoop )
{
    mLoopList.push_back( iLoop );
}

void
FVectorSection::RemoveLoop( FVectorLoop* iLoop )
{
    mLoopList.remove( iLoop );
}

std::list<FVectorLoop*>&
FVectorSection::GetLoopList()
{
    return mLoopList;
}
