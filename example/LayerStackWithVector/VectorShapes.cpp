// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VectorShapes.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "VectorShapes.h"

// IVectorShape
IVectorShape::~IVectorShape() 
{
}

IVectorShape::IVectorShape()
    : mVectorPaintingAttribute( eVectorPaintingAttribute::kStrokeAndFill )
    , mStrokeCompOp( BL_COMP_OP_SRC_OVER )
    , mStrokeStyle( BLRgba( 0.f, 0.f, 0.f ) )
    , mStrokeOptions( BLStrokeOptions() )
    , mFillCompOp( BL_COMP_OP_SRC_OVER )
    , mFillStyle( BLRgba( 0.f, 0.f, 0.f ) )
    , mFillRule( BL_FILL_RULE_NON_ZERO )
    , mTransform( BLMatrix2D::makeIdentity() )
{
    mStrokeOptions.startCap = BL_STROKE_CAP_BUTT;
    mStrokeOptions.endCap = BL_STROKE_CAP_BUTT;
    mStrokeOptions.join = BL_STROKE_JOIN_MITER_CLIP;
    mStrokeOptions.transformOrder = BL_STROKE_TRANSFORM_ORDER_AFTER;
    mStrokeOptions.width = 1.0;
    mStrokeOptions.miterLimit = 4.0;
    mStrokeOptions.dashOffset = 0.0;
}

uint8_t
IVectorShape::VectorPaintingAttribute() const {
    return  mVectorPaintingAttribute;
}

BLCompOp
IVectorShape::StrokeCompOp() const {
    return  mStrokeCompOp;
}

const BLStyle&
IVectorShape::StrokeStyle() const {
    return  mStrokeStyle;
}

const BLStrokeOptions&
IVectorShape::StrokeOptions() const {
    return  mStrokeOptions;
}

BLCompOp
IVectorShape::FillCompOp() const {
    return  mFillCompOp;
}

const BLStyle&
IVectorShape::FillStyle() const {
    return  mFillStyle;
}

BLFillRule
IVectorShape::FillRule() const {
    return  mFillRule;
}

const BLMatrix2D&
IVectorShape::Transform() const {
    return  mTransform;
}

void
IVectorShape::SetVectorPaintingAttribute( uint8_t iVectorPaintingAttribute ) {
    mVectorPaintingAttribute = iVectorPaintingAttribute;
}

void
IVectorShape::SetStrokeCompOp( BLCompOp iCompOp ) {
    mStrokeCompOp = iCompOp;
}

void
IVectorShape::SetStrokeStyle( BLStyle iStyle ) {
    mStrokeStyle = iStyle;
}

void
IVectorShape::SetStrokeOptions( BLStrokeOptions iOptions ) {
    mStrokeOptions = iOptions;
}

void
IVectorShape::SetFillCompOp( BLCompOp iCompOp ) {
    mFillCompOp = iCompOp;
}

void
IVectorShape::SetFillStyle( BLStyle iStyle ) {
    mFillStyle = iStyle;
}

void
IVectorShape::SetFillRule( BLFillRule iRule ) {
    mFillRule = iRule;
}

void
IVectorShape::SetTransform( const BLMatrix2D& iTransform ) {
    mTransform = iTransform;
}

void
IVectorShape::ResetAttributes()
{
    // Reset all visual attributes just as if it was just created but
    // retain the vector geometry data in the children.
    mVectorPaintingAttribute = eVectorPaintingAttribute::kStrokeAndFill;
    mStrokeCompOp = BL_COMP_OP_SRC_OVER;
    mStrokeStyle = BLRgba( 0.f, 0.f, 0.f );
    mStrokeOptions = BLStrokeOptions();
    mFillCompOp = BL_COMP_OP_SRC_OVER;
    mFillStyle = BLRgba( 0.f, 0.f, 0.f );
    mFillRule = BL_FILL_RULE_NON_ZERO;
    mStrokeOptions.startCap = BL_STROKE_CAP_BUTT;
    mStrokeOptions.endCap = BL_STROKE_CAP_BUTT;
    mStrokeOptions.join = BL_STROKE_JOIN_MITER_CLIP;
    mStrokeOptions.transformOrder = BL_STROKE_TRANSFORM_ORDER_AFTER;
    mStrokeOptions.width = 1.0;
    mStrokeOptions.miterLimit = 4.0;
    mStrokeOptions.dashOffset = 0.0;
    mTransform = BLMatrix2D::makeIdentity();
}

//static
void
IVectorShape::SetContextAttributesForStroke( BLContext& iCtx, const IVectorShape& iPrim )
{
    iCtx.setCompOp( iPrim.FillCompOp() );
    iCtx.setFillStyle( iPrim.FillStyle() );
    iCtx.setFillRule( iPrim.FillRule() );
}

//static
void
IVectorShape::SetContextAttributesForFill( BLContext& iCtx, const IVectorShape& iPrim )
{
    iCtx.setCompOp( iPrim.StrokeCompOp() );
    iCtx.setStrokeStyle( iPrim.StrokeStyle() );
    iCtx.setStrokeOptions( iPrim.StrokeOptions() );
}

// FRectangleVectorShape
FRectangleVectorShape::~FRectangleVectorShape()
{
}

FRectangleVectorShape::FRectangleVectorShape( const BLRect& iData )
    : IVectorShape()
    , mData( iData )
{}

BLRect&
FRectangleVectorShape::Rectangle() {
    return  mData;
}

const BLRect&
FRectangleVectorShape::Rectangle() const {
    return  mData;
}

void
FRectangleVectorShape::SetRectangle( const BLRect& iData ) {
    mData = iData;
}

// FCircleVectorShape
FCircleVectorShape::~FCircleVectorShape()
{
}

FCircleVectorShape::FCircleVectorShape( const BLCircle& iData )
    : IVectorShape()
    , mData( iData )
{}

BLCircle&
FCircleVectorShape::Circle() {
    return  mData;
}

const BLCircle&
FCircleVectorShape::Circle() const {
    return  mData;
}

void
FCircleVectorShape::SetCircle( const BLCircle& iCircle ) {
    mData = iCircle;
}

// FPathVectorShape
FPathVectorShape::~FPathVectorShape()
{
}

FPathVectorShape::FPathVectorShape( const BLPath& iData )
    : IVectorShape()
    , mData( iData )
{}

BLPath&
FPathVectorShape::Path() {
    return  mData;
}

const BLPath&
FPathVectorShape::Path() const {
    return  mData;
}

void
FPathVectorShape::SetPath( const BLPath& iPath ) {
    mData = iPath;
}

