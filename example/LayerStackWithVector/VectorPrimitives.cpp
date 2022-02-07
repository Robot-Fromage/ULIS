// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VectorPrimitives.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "VectorPrimitives.h"

IVectorPrimitive::~IVectorPrimitive() 
{
}

IVectorPrimitive::IVectorPrimitive( char iType )
    : mType( iType )
    , mVectorPaintingAttribute( eVectorPaintingAttribute::kStrokeAndFill )
    , mStrokeCompOp( BL_COMP_OP_SRC_OVER )
    , mStrokeStyle( BLRgba( 0.f, 0.f, 0.f ) )
    , mStrokeOptions( BLStrokeOptions() )
    , mFillCompOp( BL_COMP_OP_SRC_OVER )
    , mFillStyle( BLRgba( 0.f, 0.f, 0.f ) )
    , mFillRule( BL_FILL_RULE_NON_ZERO )
{
    mStrokeOptions.startCap = BL_STROKE_CAP_BUTT;
    mStrokeOptions.endCap = BL_STROKE_CAP_BUTT;
    mStrokeOptions.join = BL_STROKE_JOIN_MITER_CLIP;
    mStrokeOptions.transformOrder = BL_STROKE_TRANSFORM_ORDER_AFTER;
    mStrokeOptions.width = 1.0;
    mStrokeOptions.miterLimit = 4.0;
    mStrokeOptions.dashOffset = 0.0;
}

char
IVectorPrimitive::Type() const {
    return  mType;
}

uint8_t
IVectorPrimitive::VectorPaintingAttribute() const {
    return  mVectorPaintingAttribute;
}

BLCompOp
IVectorPrimitive::StrokeCompOp() const {
    return  mStrokeCompOp;
}

const BLStyle&
IVectorPrimitive::StrokeStyle() const {
    return  mStrokeStyle;
}

const BLStrokeOptions&
IVectorPrimitive::StrokeOptions() const {
    return  mStrokeOptions;
}

BLCompOp
IVectorPrimitive::FillCompOp() const {
    return  mFillCompOp;
}

const BLStyle&
IVectorPrimitive::FillStyle() const {
    return  mFillStyle;
}

BLFillRule
IVectorPrimitive::FillRule() const {
    return  mFillRule;
}

void
IVectorPrimitive::SetVectorPaintingAttribute( uint8_t iVectorPaintingAttribute ) {
    mVectorPaintingAttribute = iVectorPaintingAttribute;
}

void
IVectorPrimitive::SetStrokeCompOp( BLCompOp iCompOp ) {
    mStrokeCompOp = iCompOp;
}

void
IVectorPrimitive::SetStrokeStyle( BLStyle iStyle ) {
    mStrokeStyle = iStyle;
}

void
IVectorPrimitive::SetStrokeOptions( BLStrokeOptions iOptions ) {
    mStrokeOptions = iOptions;
}

void
IVectorPrimitive::SetFillCompOp( BLCompOp iCompOp ) {
    mFillCompOp = iCompOp;
}

void
IVectorPrimitive::SetFillStyle( BLStyle iStyle ) {
    mFillStyle = iStyle;
}

void
IVectorPrimitive::SetFillRule( BLFillRule iRule ) {
    mFillRule = iRule;
}

void
IVectorPrimitive::ResetAttributes()
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
}

//static
void
IVectorPrimitive::SetContextAttributesForStroke( BLContext& iCtx, const IVectorPrimitive& iPrim )
{
    iCtx.setCompOp( iPrim.FillCompOp() );
    iCtx.setFillStyle( iPrim.FillStyle() );
    iCtx.setFillRule( iPrim.FillRule() );
}

//static
void
IVectorPrimitive::SetContextAttributesForFill( BLContext& iCtx, const IVectorPrimitive& iPrim )
{
    iCtx.setCompOp( iPrim.StrokeCompOp() );
    iCtx.setStrokeStyle( iPrim.StrokeStyle() );
    iCtx.setStrokeOptions( iPrim.StrokeOptions() );
}
