#include <blend2d.h>
#include "Vector/Vector.h"

// https://stackoverflow.com/a/27863181
// https://stackoverflow.com/questions/1734745/how-to-create-circle-with-b%c3%a9zier-curves

#define MAGICRATIO 0.55191502449 // = 4*(sqrt(2)-1)/3

FVectorCircle::~FVectorCircle()
{
}

FVectorCircle::FVectorCircle( std::string iName )
    : FVectorPathCubic(iName)
{
    mRadiusX = mRadiusY = 0.0f;

    mCubicPoint[0] = new FVectorPointCubic( 0.0f, 0.0f );
    mCubicPoint[1] = new FVectorPointCubic( 0.0f, 0.0f );
    mCubicPoint[2] = new FVectorPointCubic( 0.0f, 0.0f );
    mCubicPoint[3] = new FVectorPointCubic( 0.0f, 0.0f );

    mCubicSegment[0] = new FVectorSegmentCubic( static_cast<FVectorPathCubic&>(*this), mCubicPoint[0], mCubicPoint[1] );
    mCubicSegment[1] = new FVectorSegmentCubic( static_cast<FVectorPathCubic&>(*this), mCubicPoint[1], mCubicPoint[2] );
    mCubicSegment[2] = new FVectorSegmentCubic( static_cast<FVectorPathCubic&>(*this), mCubicPoint[2], mCubicPoint[3] );
    mCubicSegment[3] = new FVectorSegmentCubic( static_cast<FVectorPathCubic&>(*this), mCubicPoint[3], mCubicPoint[0] );

    AddPoint ( mCubicPoint[0] );
    AddPoint ( mCubicPoint[1] );
    AddPoint ( mCubicPoint[2] );
    AddPoint ( mCubicPoint[3] );

    AddSegment ( mCubicSegment[0] );
    AddSegment ( mCubicSegment[1] );
    AddSegment ( mCubicSegment[2] );
    AddSegment ( mCubicSegment[3] );
}

FVectorCircle::FVectorCircle( std::string iName, double iRadius )
    : FVectorCircle(iName)
{
    SetRadius ( iRadius, iRadius );
}

FVectorCircle::FVectorCircle( std::string iName, double iRadiusX, double iRadiusY )
    : FVectorCircle(iName)
{
    SetRadius ( iRadiusX, iRadiusY );
}

void
FVectorCircle::UpdateShape()
{
    double ctlDistX = mRadiusX * MAGICRATIO;
    double ctlDistY = mRadiusY * MAGICRATIO;

    mCubicPoint[0]->Set(  0.0f    ,  mRadiusY );
    mCubicPoint[1]->Set(  mRadiusX,  0.0f     );
    mCubicPoint[2]->Set(  0.0f    , -mRadiusY );
    mCubicPoint[3]->Set( -mRadiusX,  0.0f     );

    mCubicSegment[0]->GetControlPoint(0).Set(  ctlDistX,  mRadiusY );
    mCubicSegment[0]->GetControlPoint(1).Set(  mRadiusX,  ctlDistY );

    mCubicSegment[1]->GetControlPoint(0).Set(  mRadiusX, -ctlDistY );
    mCubicSegment[1]->GetControlPoint(1).Set(  ctlDistX, -mRadiusY );

    mCubicSegment[2]->GetControlPoint(0).Set( -ctlDistX, -mRadiusY );
    mCubicSegment[2]->GetControlPoint(1).Set( -mRadiusX, -ctlDistY );

    mCubicSegment[3]->GetControlPoint(0).Set( -mRadiusX,  ctlDistY );
    mCubicSegment[3]->GetControlPoint(1).Set( -ctlDistX,  mRadiusY );

    mCubicSegment[0]->Update();
    mCubicSegment[1]->Update();
    mCubicSegment[2]->Update();
    mCubicSegment[3]->Update();
}

FVectorObject*
FVectorCircle::CopyShape()
{
    FVectorCircle* circleCopy = new FVectorCircle ( circleCopy->mName, mRadiusX, mRadiusY );

    return static_cast<FVectorObject*>( circleCopy );
}

FVectorPathCubic*
FVectorCircle::Convert()
{
    return static_cast<FVectorPathCubic*>(this->Copy());
}

void
FVectorCircle::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
    if ( mRadiusX && mRadiusY )
    {
        FVectorPathCubic::DrawShape ( iBlock, iBLContext, iRoi );
    }
}

FVectorObject*
FVectorCircle::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    if( FMath::Sqrt((iX*iX) + (iY*iY)) <= mRadiusX )
    {
        return this;
    }

    return nullptr;
}

void
FVectorCircle::SetRadius( double iRadius )
{
    SetRadius( iRadius, iRadius );
}

void
FVectorCircle::SetRadius( double iRadiusX, double iRadiusY )
{
    mRadiusX = iRadiusX;
    mRadiusY = iRadiusY;

    mBBox.x = -mRadiusX - mStrokeWidth;
    mBBox.y = -mRadiusY - mStrokeWidth;
    mBBox.w =  ( mRadiusX +  mStrokeWidth ) * 2;
    mBBox.h =  ( mRadiusY +  mStrokeWidth ) * 2;

    UpdateShape();
}

double
FVectorCircle::GetRadiusX()
{
    return mRadiusX;
}

double
FVectorCircle::GetRadiusY()
{
    return mRadiusY;
}
