#include <blend2d.h>
#include "Vector/Vector.h"

FVectorPathLoop::~FVectorPathLoop()
{
}

FVectorPathLoop::FVectorPathLoop( FVectorPath& iParent
                                , uint64 iID
                                , std::list<FVectorPoint*>& iLoopPointist
                                , FVectorPoint* iLoopPoint )
    : FVectorPath()
    , mID ( iID )
{
    mParent = &iParent;

    ShapeFromPointList ( iLoopPointist, iLoopPoint );

    Build();
printf("new loop of size : %d - ID: %d\n", mPointList.size(), iID );
}

uint64
FVectorPathLoop::GetID()
{
    return mID;
}

void
FVectorPathLoop::Invalidate()
{
    static_cast<FVectorPath*>(mParent)->InvalidateLoop( this );
}

void
FVectorPathLoop::UpdateShape()
{
    Build();
}

FVectorObject*
FVectorPathLoop::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
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
FVectorPathLoop::GenerateID( std::list<FVectorPoint*> iLoopPointist, FVectorPoint* iLoopPoint )
{
    bool started = false;
    uint64 loopID = 0;

    for( std::list<FVectorPoint*>::iterator it = iLoopPointist.begin(); it != iLoopPointist.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        if ( point == iLoopPoint )
        {
            started = true;
        }

        if ( started == true )
        {
            loopID = loopID ^ ( uint64 ) point;
        }
    }

    return loopID;
}

void
FVectorPathLoop::ShapeFromPointList( std::list<FVectorPoint*> iLoopPointist, FVectorPoint* iLoopPoint )
{
    bool started = false;
    FVectorPointIntersection* higherPoint = nullptr;

    for( std::list<FVectorPoint*>::iterator it = iLoopPointist.begin(); it != iLoopPointist.end(); ++it )
    {
        FVectorPoint* point = static_cast<FVectorPoint*>(*it);

        if ( point == iLoopPoint )
        {
            started = true;
        }
       
        if ( started == true )
        {
            AppendPoint( point );

            if ( point->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
            {
                FVectorPointIntersection* intersectionPoint = static_cast<FVectorPointIntersection*>(point);

                intersectionPoint->AddLoop( this );

                if ( intersectionPoint > higherPoint )
                {
                    higherPoint = intersectionPoint;
                }
            }
        }
    }
}

void
FVectorPathLoop::BuildSegmentCubic( std::vector<BLPoint>& iPointArray
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
FVectorPathLoop::DrawPoints( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    iBLContext.setStrokeStyle( BLRgba32( 0xFFFF8000 ) );
    iBLContext.setFillStyle( BLRgba32( 0xFFFF8000 ) );

    if ( mPointList.size() ) 
    {
        std::list<FVectorPoint*>::iterator it;

        for( it = mPointList.begin(); it != mPointList.end(); ++it )
        {
            FVectorPoint* point = static_cast<FVectorPoint*>(*it);
            FVec2D pointAt = point->GetCoords();

            if ( point->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
            {
                pointAt = static_cast<FVectorPointIntersection*>(point)->GetPosition(*point->GetFirstSegment());
            }

            iBLContext.fillRect ( pointAt.x - 5, pointAt.y - 5, 10, 10 );
        }
    }
}

void
FVectorPathLoop::Build()
{
    int seg = 0;

    mPath.clear();
/*
     mPointArray.clear();
     mPointArray.reserve(200);
*/
    if ( mPointList.size() ) 
    {
        FVectorPoint* firstPoint = mPointList.front();
        FVectorPoint* lastPoint = mPointList.back();
        std::list<FVectorPoint*>::iterator     it;
        std::list<FVectorPoint*>::iterator nextit;

        FVec2D firstAt = static_cast<FVectorPointIntersection*>(firstPoint)->GetPosition(*firstPoint->GetFirstSegment());

        mPath.moveTo( firstAt.x, firstAt.y );

        for( it = mPointList.begin(), nextit = mPointList.begin(); it != mPointList.end(); ++it )
        {
            FVectorPoint* point = static_cast<FVectorPoint*>(*it);
            FVectorPoint* nextPoint = ( point == lastPoint ) ? firstPoint : static_cast<FVectorPoint*>(*(++nextit));

            if ( (     point->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION )
              && ( nextPoint->GetType() == FVectorPoint::POINT_TYPE_INTERSECTION ) )
            {
                FVectorSegmentCubic* cubicSegment = static_cast<FVectorSegmentCubic*>(point->GetSegment( *nextPoint ));

                if ( cubicSegment )
                {
                    double     pointT =     static_cast<FVectorPointIntersection*>(point)->GetT(*cubicSegment);
                    double nextPointT = static_cast<FVectorPointIntersection*>(nextPoint)->GetT(*cubicSegment);

                    FVec2D     pointAt = static_cast<FVectorPointIntersection*>(point)->GetPosition(*cubicSegment);
                    FVec2D nextPointAt = static_cast<FVectorPointIntersection*>(nextPoint)->GetPosition(*cubicSegment);

                    BuildSegmentCubic ( mPointArray, *cubicSegment, pointT, nextPointT );
                }
            }
        }

        mPath.close();
    }
}


void
FVectorPathLoop::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    if ( IsFilled() )
    {
    /*if ( mPointList.size() ) 
    {*/
       iBLContext.setFillStyle( BLRgba32( mFillColor ) );

       /*iBLContext.fillPolygon( &mPointArray[0], mPointArray.size() );*/
       iBLContext.fillPath( mPath );
    /*}*/
    }
}
