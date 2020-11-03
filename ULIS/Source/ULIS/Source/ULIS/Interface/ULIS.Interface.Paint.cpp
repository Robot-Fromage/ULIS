// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.Paint.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FPainterContext class.
 */
#include "ULIS/Interface/ULIS.Interface.Paint.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Paint/ULIS.Paint.PainterContext.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FPainterContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
void
FPainterContext::DrawLine( IBlock* iBlock
                         , const FPoint& iP0
                         , const FPoint& iP1
                         , const CColor& iColor
                         , const FRect& iClippingRect
                         , const FPerformanceOptions& iPerformanceOptions
                         , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawLine( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                            \
                                                          , iP0, iP1, iColor, iClippingRect, iPerformanceOptions, iCallInvalidCB );             \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawLineAA( IBlock* iBlock
                           , const FPoint& iP0
                           , const FPoint& iP1
                           , const CColor& iColor
                           , const FRect& iClippingRect
                           , const FPerformanceOptions& iPerformanceOptions
                           , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawLineAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                          \
                                                            , iP0, iP1, iColor, iClippingRect, iPerformanceOptions, iCallInvalidCB );           \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawCircleAndres( IBlock* iBlock
                                 , const FPoint& iCenter
                                 , int iRadius
                                 , const CColor& iColor
                                 , bool iFilled
                                 , const FRect& iClippingRect
                                 , const FPerformanceOptions& iPerformanceOptions
                                 , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleAndres( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                    \
                                                                  , iCenter, iRadius, iColor, iFilled, iClippingRect                            \
                                                                  , iPerformanceOptions, iCallInvalidCB );                                      \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawCircleAndresAA( IBlock* iBlock
                                   , const FPoint& iCenter
                                   , int iRadius
                                   , const CColor& iColor
                                   , bool iFilled
                                   , const FRect& iClippingRect
                                   , const FPerformanceOptions& iPerformanceOptions
                                   , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleAndresAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                  \
                                                                    , iCenter, iRadius, iColor, iFilled, iClippingRect                          \
                                                                    , iPerformanceOptions, iCallInvalidCB );                                    \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawCircleBresenham( IBlock* iBlock
                                    , const FPoint& iCenter
                                    , int iRadius
                                    , const CColor& iColor
                                    , bool iFilled
                                    , const FRect& iClippingRect
                                    , const FPerformanceOptions& iPerformanceOptions
                                    , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                     \
            {                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleBresenham( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                     \
                                                                     , iCenter, iRadius, iColor, iFilled, iClippingRect                             \
                                                                     , iPerformanceOptions, iCallInvalidCB );                                       \
                break;                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawCircleBresenhamAA( IBlock* iBlock
                                      , const FPoint& iCenter
                                      , int iRadius
                                      , const CColor& iColor
                                      , bool iFilled
                                      , const FRect& iClippingRect
                                      , const FPerformanceOptions& iPerformanceOptions
                                      , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                     \
            {                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleBresenhamAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                   \
                                                                       , iCenter, iRadius, iColor, iFilled, iClippingRect                           \
                                                                       , iPerformanceOptions, iCallInvalidCB );                                     \
                break;                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawEllipse( IBlock* iBlock
                            , const FPoint& iCenter
                            , int iA
                            , int iB
                            , const CColor& iColor
                            , bool iFilled
                            , const FRect& iClippingRect
                            , const FPerformanceOptions& iPerformanceOptions
                            , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                TPainterContext< ULIS_REG[ n ] >::DrawEllipse( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                     \
                                                             , iCenter, iA, iB, iColor, iFilled, iClippingRect                              \
                                                             , iPerformanceOptions, iCallInvalidCB );                                       \
                break;                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawEllipseAA( IBlock* iBlock
                              , const FPoint& iCenter
                              , int iA
                              , int iB
                              , const CColor& iColor
                              , bool iFilled
                              , const FRect& iClippingRect
                              , const FPerformanceOptions& iPerformanceOptions
                              , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                TPainterContext< ULIS_REG[ n ] >::DrawEllipseAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                   \
                                                               , iCenter, iA, iB, iColor, iFilled, iClippingRect                            \
                                                               , iPerformanceOptions, iCallInvalidCB );                                     \
                break;                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawRotatedEllipse( IBlock* iBlock
                                   , const FPoint& iCenter
                                   , int iA
                                   , int iB
                                   , int iRotationDegrees
                                   , const CColor& iColor
                                   , bool iFilled
                                   , const FRect& iClippingRect
                                   , const FPerformanceOptions& iPerformanceOptions
                                   , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                                     \
            {                                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawRotatedEllipse( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                      \
                                                                    , iCenter, iA, iB, iRotationDegrees, iColor, iFilled                                            \
                                                                    , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                         \
                break;                                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawRotatedEllipseAA( IBlock* iBlock
                                     , const FPoint& iCenter
                                     , int iA
                                     , int iB
                                     , int iRotationDegrees
                                     , const CColor& iColor
                                     , bool iFilled
                                     , const FRect& iClippingRect
                                     , const FPerformanceOptions& iPerformanceOptions
                                     , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                                     \
            {                                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawRotatedEllipseAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                    \
                                                                      , iCenter, iA, iB, iRotationDegrees, iColor, iFilled                                          \
                                                                      , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                       \
                break;                                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawArcAndres( IBlock* iBlock
                              , const FPoint& iCenter
                              , int iRadius
                              , int iStartDegree
                              , int iEndDegree
                              , const CColor& iColor
                              , const FRect& iClippingRect
                              , const FPerformanceOptions& iPerformanceOptions
                              , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                                 \
            {                                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawArcAndres( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                       \
                                                               , iCenter, iRadius, iStartDegree, iEndDegree, iColor                                             \
                                                               , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                          \
                break;                                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    

//static
void
FPainterContext::DrawArcAndresAA( IBlock* iBlock
                                , const FPoint& iCenter
                                , int iRadius
                                , int iStartDegree
                                , int iEndDegree
                                , const CColor& iColor
                                , const FRect& iClippingRect
                                , const FPerformanceOptions& iPerformanceOptions
                                , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                                 \
            {                                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawArcAndresAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                     \
                                                                 , iCenter, iRadius, iStartDegree, iEndDegree, iColor                                           \
                                                                 , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                        \
                break;                                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawArcBresenham( IBlock* iBlock
                                 , const FPoint& iCenter
                                 , int iRadius
                                 , int iStartDegree
                                 , int iEndDegree
                                 , const CColor& iColor
                                 , const FRect& iClippingRect
                                 , const FPerformanceOptions& iPerformanceOptions
                                 , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                                 \
            {                                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawArcBresenham( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                    \
                                                                  , iCenter, iRadius, iStartDegree, iEndDegree, iColor                                          \
                                                                  , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                       \
                break;                                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawArcBresenhamAA( IBlock* iBlock
                                   , const FPoint& iCenter
                                   , int iRadius
                                   , int iStartDegree
                                   , int iEndDegree
                                   , const CColor& iColor
                                   , const FRect& iClippingRect
                                   , const FPerformanceOptions& iPerformanceOptions
                                   , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                                     \
            {                                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawArcBresenhamAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                      \
                                                                    , iCenter, iRadius, iStartDegree, iEndDegree, iColor                                            \
                                                                    , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                         \
                break;                                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawRectangle( IBlock* iBlock
                              , const FPoint& iTopLeft
                              , const FPoint& iBottomRight
                              , const CColor& iColor
                              , bool iFilled
                              , const FRect& iClippingRect
                              , const FPerformanceOptions& iPerformanceOptions
                              , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                     \
            {                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawRectangle( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                           \
                                                               , iTopLeft, iBottomRight, iColor, iFilled                                            \
                                                               , iClippingRect, iPerformanceOptions, iCallInvalidCB );                              \
                break;                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawPolygon( IBlock* iBlock
                            , std::vector< FPoint >& iPoints
                            , const CColor& iColor
                            , bool iFilled
                            , const FRect& iClippingRect
                            , const FPerformanceOptions& iPerformanceOptions
                            , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                    \
            case ULIS_REG[ n ]:                                                                                                     \
            {                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawPolygon( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                             \
                                                             , iPoints, iColor, iFilled                                            \
                                                             , iClippingRect, iPerformanceOptions, iCallInvalidCB );                \
                break;                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawPolygonAA( IBlock* iBlock
                              , std::vector< FPoint >& iPoints
                              , const CColor& iColor
                              , bool iFilled
                              , const FRect& iClippingRect
                              , const FPerformanceOptions& iPerformanceOptions
                              , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                    \
            case ULIS_REG[ n ]:                                                                                                     \
            {                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawPolygonAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                           \
                                                               , iPoints, iColor, iFilled                                           \
                                                               , iClippingRect, iPerformanceOptions, iCallInvalidCB );              \
                break;                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawQuadraticBezier( IBlock* iBlock
                                    , const FPoint& iCtrlPt0
                                    , const FPoint& iCtrlPt1
                                    , const FPoint& iCtrlPt2
                                    , float iWeight
                                    , const CColor& iColor
                                    , const FRect& iClippingRect
                                    , const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                                 \
            {                                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawQuadraticBezier( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                                 \
                                                                     , iCtrlPt0, iCtrlPt1, iCtrlPt2, iWeight, iColor                                            \
                                                                     , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                    \
                break;                                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void FPainterContext::DrawQuadraticBezierAA( IBlock* iBlock
                                           , const FPoint& iCtrlPt0
                                           , const FPoint& iCtrlPt1
                                           , const FPoint& iCtrlPt2
                                           , float iWeight
                                           , const CColor& iColor
                                           , const FRect& iClippingRect
                                           , const FPerformanceOptions& iPerformanceOptions
                                           , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                                 \
            {                                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawQuadraticBezierAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                               \
                                                                       , iCtrlPt0, iCtrlPt1, iCtrlPt2, iWeight, iColor                                          \
                                                                       , iClippingRect, iPerformanceOptions, iCallInvalidCB );                                  \
                break;                                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}

} // namespace ULIS

