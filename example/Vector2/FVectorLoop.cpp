#include <blend2d.h>
#include "Vector/Vector.h"

FVectorLoop::~FVectorLoop()
{
}

FVectorLoop::FVectorLoop( FVectorPath& iParent
                        , uint64 iID
                        , FVectorPoint& iLoopPoint
                        , std::list<FVectorPoint*>& iLoopPointList
                        , std::list<FVectorSection*>& iLoopSectionList )
    : mID ( iID )
    , mLoopPoint ( iLoopPoint )
    , mPointList ( iLoopPointList )
    , mSectionList ( iLoopSectionList )
{
    mParent = static_cast<FVectorObject*>(&iParent);

    Build();
printf("new loop - ID : %d - Vertices : %d - Sections: %d\n", mID, mPointList.size(), mSectionList.size() );
}

uint64
FVectorLoop::GetID()
{
    return mID;
}

void
FVectorLoop::Invalidate()
{
    static_cast<FVectorPath*>(mParent)->InvalidateLoop( this );
}

void
FVectorLoop::UpdateShape()
{
    Build();
}

FVectorObject*
FVectorLoop::CopyShape()
{
    return nullptr;
}

FVectorObject*
FVectorLoop::PickShape( double iX, double iY, double iRadius )
{
    /*BLPath path;*/
    BLPoint p = { iX, iY };
    BLBox bbox;

/*
    if ( mPointArray.size() )
    {
        path.moveTo ( mPointArray[0].x, mPointArray[0].y );

        for ( int i = 1; i < mPointArray.size(); i++ )
        {
            path.lineTo ( mPointArray[i].x, mPointArray[i].y );
        }

        path.lineTo ( mPointArray[0].x, mPointArray[0].y );
    }
*/
    mPath.getBoundingBox( &bbox );

    if ( ( iX > bbox.x0 ) && ( iX < bbox.x1 ) && ( iY > bbox.y0 ) && ( iY < bbox.y1 ) )
    {
        return this;
    }

/*
    if ( mPath.hitTest( p, BL_FILL_RULE_NON_ZERO ) == BL_HIT_TEST_IN  )
    {
printf("%d\n", mID );
        return this;
    }
*/
    return nullptr;
};

uint64
FVectorLoop::GenerateID( std::list<FVectorSection*> iSectionList )
{
    uint64 loopID = 0;

    for( std::list<FVectorSection*>::iterator it = iSectionList.begin(); it != iSectionList.end(); ++it )
    {
        FVectorSection* section = static_cast<FVectorSection*>(*it);

        loopID = loopID ^ ( uint64 ) section;
    }

    return loopID;
}

void
FVectorLoop::Detach()
{
    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        point->RemoveLoop( this );
    }

    for( std::list<FVectorSection*>::iterator it = mSectionList.begin(); it != mSectionList.end(); ++it )
    {
        FVectorSection* section = static_cast<FVectorSection*>(*it);

        section->RemoveLoop( this );
    }
}

void
FVectorLoop::Attach()
{
    for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        point->AddLoop( this );
    }

    for( std::list<FVectorSection*>::iterator it = mSectionList.begin(); it != mSectionList.end(); ++it )
    {
        FVectorSection* section = static_cast<FVectorSection*>(*it);

        section->AddLoop( this );
    }
}

void
FVectorLoop::BuildSegmentCubic( std::vector<BLPoint>& iPointArray
                              , FVectorSegmentCubic& iSegment
                              , double iFromT
                              , double iToT )
{
    std::vector<FPolygon>& polygonCache = iSegment.GetPolygonCache();
    uint32 polyCount = iSegment.GetPolygonCount();
    bool revert = ( iFromT < iToT ) ? false : true;

    if ( revert == false )
    {
        for( int i = 0; i < polyCount; i++ )
        {
            if( polygonCache[i].toT >= iFromT && polygonCache[i].fromT <= iToT )
            {
                FVec2D to   = { polygonCache[i].lineVertex[1].x, polygonCache[i].lineVertex[1].y };
                FVec2D from = { polygonCache[i].lineVertex[0].x, polygonCache[i].lineVertex[0].y };

                // clipping part
                if ( polygonCache[i].toT > iToT )
                {
                    FVec2D dir = polygonCache[i].lineVertex[1] - polygonCache[i].lineVertex[0];

                    to.x = polygonCache[i].lineVertex[0].x + dir.x * ( ( iToT - polygonCache[i].fromT ) / ( polygonCache[i].toT - polygonCache[i].fromT ) );
                    to.y = polygonCache[i].lineVertex[0].y + dir.y * ( ( iToT - polygonCache[i].fromT ) / ( polygonCache[i].toT - polygonCache[i].fromT ) );
                }

                BLPoint p = { to.x, to.y };

                /*iPointArray.push_back(p);*/

                mPath.lineTo( p );
            }
        }
    }
    else
    {
        double tmp = iFromT;
        iFromT = iToT;
        iToT = tmp;

        for( int i = polyCount - 1; i > 0; i-- )
        {
            if( polygonCache[i].toT >= iFromT && polygonCache[i].fromT <= iToT )
            {
                FVec2D to   = { polygonCache[i].lineVertex[1].x, polygonCache[i].lineVertex[1].y };
                FVec2D from = { polygonCache[i].lineVertex[0].x, polygonCache[i].lineVertex[0].y };

                // clipping part
                if ( polygonCache[i].fromT < iFromT )
                {
                    FVec2D dir = polygonCache[i].lineVertex[1] - polygonCache[i].lineVertex[0];

                    from.x = polygonCache[i].lineVertex[0].x + dir.x * ( ( iFromT - polygonCache[i].fromT ) / ( polygonCache[i].toT - polygonCache[i].fromT ) );
                    from.y = polygonCache[i].lineVertex[0].y + dir.y * ( ( iFromT - polygonCache[i].fromT ) / ( polygonCache[i].toT - polygonCache[i].fromT ) );
                }

                BLPoint p = { from.x, from.y };

                /*iPointArray.push_back(p);*/

                mPath.lineTo( p );
            }
        }
    }
}

void
FVectorLoop::DrawPoints( FRectD& iRoi )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    blctx.setStrokeStyle( BLRgba32( 0xFFFF8000 ) );
    blctx.setFillStyle( BLRgba32( 0xFFFF8000 ) );

    if ( mPointList.size() ) 
    {
        for( std::list<FVectorPoint*>::iterator it = mPointList.begin(); it != mPointList.end(); ++it )
        {
            FVectorPoint* point = static_cast<FVectorPoint*>(*it);
            FVec2D& pointAt = point->GetCoords();

            blctx.fillRect ( pointAt.x - 5, pointAt.y - 5, 10, 10 );
        }
    }
}

void
FVectorLoop::Build()
{
    int seg = 0;

    mPath.clear();
/*
     mPointArray.clear();
     mPointArray.reserve(200);
*/
    if ( mSectionList.size() ) 
    {
        FVectorSection* firstSection = mSectionList.front();
        FVectorSegment& firstSegment = firstSection->GetSegment();
        FVec2D originAt = mLoopPoint.GetPosition( firstSegment );
        FVectorPoint* currentPoint = &mLoopPoint;

        mPath.moveTo( originAt.x, originAt.y );

        for( std::list<FVectorSection*>::iterator it = mSectionList.begin(); it != mSectionList.end(); ++it )
        {
            FVectorSection* section = static_cast<FVectorSection*>(*it);
            FVectorSegment& segment = section->GetSegment();
            FVectorPoint* nextPoint = ( currentPoint == section->GetPoint(0) ) ? section->GetPoint(1) : section->GetPoint(0);
            double currentPointT = currentPoint->GetT( segment );
            double    nextPointT =    nextPoint->GetT( segment );

            BuildSegmentCubic ( mPointArray, static_cast<FVectorSegmentCubic&>(segment), currentPointT, nextPointT );

            currentPoint = nextPoint;
        }

        mPath.close();
    }
}


void
FVectorLoop::DrawShape( FRectD& iRoi, uint64 iFlags )
{
    BLContext& blctx = FVectorEngine::GetBLContext();

    if ( IsFilled() )
    {
    /*if ( mPointList.size() ) 
    {*/
       blctx.setFillStyle( BLRgba32( mFillColor ) );

       /*iBLContext.fillPolygon( &mPointArray[0], mPointArray.size() );*/
       blctx.fillPath( mPath );
    /*}*/
    }
}
