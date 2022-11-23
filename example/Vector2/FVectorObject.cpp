#include <blend2d.h>
#include "Vector/Vector.h"

FVectorObject::~FVectorObject()
{
}

FVectorObject::FVectorObject()
    : mTranslation( 0, 0 )
    , mRotation( 0 )
    , mScaling ( 1.0f, 1.0f )
    , mStrokeColor ( 0xFF000000 )
    , mFillColor ( 0xFF000000 )
    , mStrokeWidth ( 4.0f )
    , mParent ( nullptr )
    , mIsFilled ( false )
    , mIsSelected ( false )
    , mIsInvalidated ( false )
{
    mLocalMatrix.reset();
    mInverseLocalMatrix.reset();
    mWorldMatrix.reset();
    mInverseWorldMatrix.reset();
}

FVectorObject::FVectorObject( std::string iName )
    : FVectorObject( )
{
    mName.assign( iName );
}

void
FVectorObject::Update()
{
    UpdateShape();

    mIsInvalidated = false;
}

void
FVectorObject::SetIsSelected( bool iIsSelected )
{
    mIsSelected = iIsSelected;
}

void
FVectorObject::Translate( double iX, double iY )
{
    mTranslation.x = iX;
    mTranslation.y = iY;
}

void
FVectorObject::Rotate( double iAngle )
{
    mRotation = iAngle;
}

void
FVectorObject::Scale( double iX, double iY )
{
    mScaling.x = iX;
    mScaling.y = iY;
}

double
FVectorObject::GetScalingX()
{
    return mScaling.x;
}

double
FVectorObject::GetScalingY()
{
    return mScaling.y;
}

double
FVectorObject::GetRotation()
{
    return mRotation;
}

FVectorObject*
FVectorObject::Copy() {
    FVectorObject* objectCopy = CopyShape();

   // TODO, update matrices once we get a BLContext object

    if( objectCopy )
    {
        CopySettings( *objectCopy ); // we need the matrices to properly import the child

        // recurse
        for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
        {
            FVectorObject *child = (*it);

            objectCopy->AppendChild( child->Copy() );
        }

        objectCopy->UpdateShape();
    }

    return objectCopy;
}

void
FVectorObject::CopySettings( FVectorObject& iDestinationObject )
{
    iDestinationObject.mTranslation = mTranslation;
    iDestinationObject.mRotation = mRotation;
    iDestinationObject.mScaling = mScaling;

    iDestinationObject.mStrokeColor = mStrokeColor;
    iDestinationObject.mStrokeWidth = mStrokeWidth;
    iDestinationObject.mFillColor = mFillColor;
    iDestinationObject.mIsFilled = mIsFilled;

    iDestinationObject.mBBox = mBBox;

    iDestinationObject.mName = mName;
    iDestinationObject.mName.append("_Copy");

    iDestinationObject.mLocalMatrix = mLocalMatrix;
    iDestinationObject.mInverseLocalMatrix = mInverseLocalMatrix;
    iDestinationObject.mWorldMatrix = mWorldMatrix;
    iDestinationObject.mInverseWorldMatrix = mInverseWorldMatrix;

    /*UpdateMatrix();*/
}

double
FVectorObject::GetTranslationX()
{
    return mTranslation.x;
}

double
FVectorObject::GetTranslationY()
{
    return mTranslation.y;
}

void
FVectorObject::UpdateMatrix( BLContext& iBLContext )
{
    iBLContext.save();
    iBLContext.resetMatrix();
    iBLContext.translate( mTranslation.x, mTranslation.y );
    iBLContext.rotate( mRotation );
    iBLContext.scale( mScaling.x, mScaling.y );
    mLocalMatrix = iBLContext.userMatrix();
    /*iBLContext.restore();

    iBLContext.save();*/

    BLMatrix2D::invert( mInverseLocalMatrix, mLocalMatrix );

    if( mParent)
    {
        iBLContext.setMatrix( mParent->mWorldMatrix );
        iBLContext.transform( mLocalMatrix );
        mWorldMatrix = iBLContext.userMatrix();

        BLMatrix2D::invert( mInverseWorldMatrix, mWorldMatrix );
    }
     else
    {
        memcpy( &mWorldMatrix       , &mLocalMatrix       , sizeof ( mLocalMatrix        ) );
        memcpy( &mInverseWorldMatrix, &mInverseLocalMatrix, sizeof ( mInverseLocalMatrix ) );
    }

    // recurse
    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);

        child->UpdateMatrix( iBLContext );
    }

    iBLContext.restore();
}

FRectD
FVectorObject::GetBBox( bool iWorld )
{

    if ( iWorld == true )
    {
        BLPoint origin = mWorldMatrix.mapPoint( mBBox.x, mBBox.y );
        BLPoint size = mWorldMatrix.mapVector( mBBox.w, mBBox.h );
        FRectD worldBBox = { origin.x, origin.y, size.x, size.y };
        
        return worldBBox;
    }
    

    return mBBox;
}

void
FVectorObject::DrawChildren( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    for( std::list<FVectorObject*>::iterator it = mChildrenList.begin(); it != mChildrenList.end(); ++it )
    {
        FVectorObject *child = (*it);

        child->Draw( iBlock, iBLContext, iRoi );
    }
}

void
FVectorObject::Draw( FBlock& iBlock, BLContext& iBLContext, FRectD& iRoi )
{
    FRectD localRoi = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Adapt the Region-Of-Interest to the local coordinates
    if( iRoi.Area() != 0.0f )
    {
        BLPoint roiCornerOrigin = { iRoi.x, iRoi.y };
        BLPoint roiCornerSize = { iRoi.w, iRoi.h };
        BLPoint localRoiCornerOrigin = mInverseLocalMatrix.mapPoint( roiCornerOrigin.x, roiCornerOrigin.y );
        BLPoint localRoiCornerSize = mInverseLocalMatrix.mapVector( roiCornerSize.x, roiCornerSize.y );

        localRoi.x = localRoiCornerOrigin.x;
        localRoi.y = localRoiCornerOrigin.y;
        localRoi.w = localRoiCornerSize.x;
        localRoi.h = localRoiCornerSize.y;
    }

            /*printf("%s : %f %f %f %f\n",mName.c_str(), iRoi.x,iRoi.y,iRoi.w,iRoi.h);*/

    iBLContext.save();
    iBLContext.transform( mLocalMatrix );
    /*iBLContext.translate( mTranslation.x, mTranslation.y );*/
    DrawShape( iBlock, iBLContext, localRoi );

    DrawChildren( iBlock, iBLContext, localRoi );

    iBLContext.restore();
}

void
FVectorObject::DrawShape( FBlock& iBlock, BLContext& iBLContext, FRectD &iRoi )
{
}

FVectorObject*
FVectorObject::PickShape( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    return nullptr;
}

bool
FVectorObject::IsFilled()
{
    return mIsFilled;
}

bool
FVectorObject::IsInvalidated()
{
    return mIsInvalidated;
}

bool
FVectorObject::IsSelected()
{
    return mIsSelected;
}

void
FVectorObject::Invalidate()
{
    if ( mIsInvalidated == false )
    {
        FVectorObject* obj = GetRoot();

        if ( obj && ( obj != this ) )
        {
            if ( typeid ( *obj ) == typeid ( FVectorRoot ) )
            {
                FVectorRoot* root = static_cast<FVectorRoot*>(obj);

                root->InvalidateObject( this );
            }
        }

        mIsInvalidated = true;
    }
}

FVectorRoot*
FVectorObject::GetRoot()
{
    FVectorObject* parent = mParent;
    FVectorObject* root = nullptr;

    while ( parent )
    {
        root = parent;

        parent = parent->GetParent();
    }

    return static_cast<FVectorRoot*>(root);
}

void
FVectorObject::SetParent( FVectorObject* iObject )
{
    mParent = iObject;
}

FVec2D
FVectorObject::WorldCoordinatesToLocal( double iX, double iY )
{
    BLPoint localCoords;
    BLPoint localSize;
    FVec2D localPoint;

    localCoords = mInverseWorldMatrix.mapPoint( iX, iY );

    localPoint.x = localCoords.x;
    localPoint.y = localCoords.y;

    return localPoint;
}

FVectorObject*
FVectorObject::GetParent()
{
    return mParent;
}

void
FVectorObject::MoveBack()
{
    if ( mParent )
    {
        std::list<FVectorObject*>::iterator it1 = mParent->mChildrenList.begin();
        std::list<FVectorObject*>::iterator it2 = it1++;

        if ( mParent->mChildrenList.size() )
        {
            for( ; it1 != mParent->mChildrenList.end(); it1++, it2++ )
            {
                FVectorObject *child = static_cast<FVectorObject*>(*it1);

                if ( this == child )
                {
                    std::swap(*it1, *it2);

                    return;
                }
            }
        }
    } 
}

void
FVectorObject::MoveFront()
{
    if ( mParent )
    {
        std::list<FVectorObject*>::iterator it2 = mParent->mChildrenList.begin();
        std::list<FVectorObject*>::iterator it1 = it2++;

        if ( mParent->mChildrenList.size() )
        {
            for( ; it2 != mParent->mChildrenList.end(); it1++, it2++ )
            {
                FVectorObject *child = static_cast<FVectorObject*>(*it1);

                if ( this == child )
                {
                    std::swap(*it1, *it2);

                    return;
                }
            }
        }
    } 
}

FVectorObject*
FVectorObject::Pick( BLContext& iBLContext, double iX, double iY, double iRadius )
{
    if ( this->mParent )
    {
        if ( typeid ( *this->mParent ) == typeid ( FVectorGroup ) )
        {
            return this->mParent;
        }
    }

    return PickShape( iBLContext, iX, iY, iRadius );
}

void
FVectorObject::ExtractTransformations( BLMatrix2D &iMatrix
                                     , FVec2D* iTranslation
                                     , double* iRotation
                                     , FVec2D* iScaling )
{
    if( iTranslation )
    {
        iTranslation->x = iMatrix.m20;
        iTranslation->y = iMatrix.m21;
    }

    if( iRotation )
    {
        *iRotation = atan( iMatrix.m01  / iMatrix.m11 );
    }

    if( iScaling )
    {
        iScaling->x = sqrt( ( iMatrix.m00 * iMatrix.m00 ) + ( iMatrix.m01 * iMatrix.m01 ) );
        iScaling->y = sqrt( ( iMatrix.m10 * iMatrix.m10 ) + ( iMatrix.m11 * iMatrix.m11 ) );
    }
}

void
FVectorObject::AppendChild( FVectorObject* iChild )
{
    AddChild ( iChild, false );
}

void
FVectorObject::PrependChild( FVectorObject* iChild )
{
    AddChild ( iChild, true );
}

void
FVectorObject::AddChild( FVectorObject* iChild, bool iPrepend )
{
    BLContext& blctx = FVectorEngine::GetBLContext();
    BLMatrix2D localMatrix = this->GetInverseWorldMatrix();

    iChild->mParent = this;

    localMatrix.transform( iChild->GetWorldMatrix() );

    ExtractTransformations ( localMatrix, &iChild->mTranslation, &iChild->mRotation, &iChild->mScaling );

    iChild->UpdateMatrix( blctx );

    if ( iPrepend == true )
    {
        mChildrenList.push_front( iChild );
    }
    else
    {
        mChildrenList.push_back( iChild );
    }
}

void
FVectorObject::RemoveChild( FVectorObject* iChild )
{
    iChild->mParent = nullptr;

    mChildrenList.remove(iChild);
}

void
FVectorObject::SetStrokeColor( uint32 iColor )
{
    mStrokeColor = iColor;
}

void
FVectorObject::SetFillColor( uint32 iColor )
{
    mFillColor = iColor;
}

void
FVectorObject::SetFilled( bool iIsFilled)
{
    mIsFilled = iIsFilled;
}

void
FVectorObject::SetStrokeWidth( double iWidth )
{
    mStrokeWidth = iWidth;
}

double
FVectorObject::GetStrokeWidth()
{
    return mStrokeWidth;
}

void 
FVectorObject::CopyTransformation( FVectorObject& iObject )
{
    iObject.mRotation    = mRotation;
    iObject.mScaling     = mScaling;
    iObject.mTranslation = mTranslation;
}

std::list<FVectorObject*>&
FVectorObject::GetChildrenList()
{
    return mChildrenList;
}

BLMatrix2D&
FVectorObject::GetLocalMatrix()
{
    return mLocalMatrix;
}

BLMatrix2D&
FVectorObject::GetWorldMatrix()
{
    return mWorldMatrix;
}

BLMatrix2D&
FVectorObject::GetInverseWorldMatrix()
{
    return mInverseWorldMatrix;
}
