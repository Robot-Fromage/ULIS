// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.Make.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FMakeContext class.
 */
#include "ULIS/Interface/ULIS.Interface.Make.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Make/ULIS.Make.MakeContext.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FMakeContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
::ULIS::IBlock*
FMakeContext::MakeBlock( int iWidth
                       , int iHeight
                       , uint32_t iFormat
                       , const std::string& iProfileTag )
{
    switch( iFormat )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ULIS_REG[ n ]: return  new ::ULIS::TBlock< ULIS_REG[ n ] >( iWidth, iHeight, iProfileTag );
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
        default: return  nullptr;
    }
}


//static
::ULIS::IBlock*
FMakeContext::MakeBlockFromExternalData( int iWidth
                                       , int iHeight
                                       , uint8* iData
                                       , uint32_t iFormat
                                       , const std::string& iProfileTag )
{
    switch( iFormat )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ULIS_REG[ n ]: return  new ::ULIS::TBlock< ULIS_REG[ n ] >( iWidth, iHeight, iData, iProfileTag );
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
        default: return  nullptr;
    }
}


//static
::ULIS::IBlock*
FMakeContext::MakeBlockFromExternalDataTakeOwnership( int iWidth
                                                    , int iHeight
                                                    , uint8* iData
                                                    , uint32_t iFormat
                                                    , const std::string& iProfileTag )
{
    switch( iFormat )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ULIS_REG[ n ]: return  new ::ULIS::TBlock< ULIS_REG[ n ] >( iWidth, iHeight, iData, iProfileTag );
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
        default: return  nullptr;
    }
}


//static
::ULIS::IBlock*
FMakeContext::MakeBlockFromDataPerformCopy( int iWidth
                                          , int iHeight
                                          , uint8* iData
                                          , uint32_t iFormat
                                          , const FPerformanceOptions& iPerformanceOptions
                                          , const std::string& iProfileTag )
{
    ::ULIS::IBlock* src = MakeBlockFromExternalData( iWidth, iHeight, iData, iFormat, iProfileTag );
    ::ULIS::IBlock* ret = MakeBlock( iWidth, iHeight, iFormat, iProfileTag );
    CopyBlockInto( src, ret );
    delete  src;
    return  ret;
}


//static
::ULIS::IBlock*
FMakeContext::CopyBlock( ::ULIS::IBlock* iBlock
                       , const FPerformanceOptions& iPerformanceOptions)
{
    return  CopyBlockRect( iBlock, FRect( 0, 0, iBlock->Width(), iBlock->Height() ), iPerformanceOptions);
}


//static
void
FMakeContext::CopyBlockInto( ::ULIS::IBlock* iSrc
                           , ::ULIS::IBlock* iDst
                           , const FPerformanceOptions& iPerformanceOptions)
{
    CopyBlockRectInto( iSrc, iDst, FRect( 0, 0, iDst->Width(), iDst->Height() ), iPerformanceOptions );
}


//static
::ULIS::IBlock*
FMakeContext::CopyBlockRect( ::ULIS::IBlock* iBlock
                           , const FRect& iRect
                           , const FPerformanceOptions& iPerformanceOptions)
{
    ::ULIS::IBlock* ret = MakeBlock( iRect.w, iRect.h, iBlock->Id() );
    CopyBlockRectInto( iBlock, ret, iRect, iPerformanceOptions );
    return  ret;
}


//static
void
FMakeContext::CopyBlockRectInto( ::ULIS::IBlock* iSrc
                               , ::ULIS::IBlock* iDst
                               , const FRect& iRect
                               , const FPerformanceOptions& iPerformanceOptions)
{
    assert( iSrc->Id() == iDst->Id() );
    switch( iSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                        ::ULIS::TMakeContext< ULIS_REG[ n ] >                                                                               \
                        ::CopyBlockRectInto( (::ULIS::TBlock< ULIS_REG[ n ] >*)iSrc                                                         \
                                           , (::ULIS::TBlock< ULIS_REG[ n ] >*)iDst                                                         \
                                           , iRect, iPerformanceOptions);                                                                   \
                        break;                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }

    if( iSrc->ColorProfile() )
        iDst->AssignColorProfile( iSrc->ColorProfile()->Name() );
}


//static
void
FMakeContext::CopyBlockRectInto( IBlock* iSrc
                               , IBlock* iDst
                               , const FRect& iSrcRect
                               , const FPoint& iDstPos
                               , const FPerformanceOptions& iPerformanceOptions )
{
    assert( iSrc->Id() == iDst->Id() );
    switch( iSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                        ::ULIS::TMakeContext< ULIS_REG[ n ] >                                                                               \
                        ::CopyBlockRectInto( (::ULIS::TBlock< ULIS_REG[ n ] >*)iSrc                                                         \
                                           , (::ULIS::TBlock< ULIS_REG[ n ] >*)iDst                                                         \
                                           , iSrcRect, iDstPos, iPerformanceOptions);                                                       \
                        break;                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
FRect
FMakeContext::GetTrimmedTransparencyRect( const IBlock* iSrc
                                        , const FPerformanceOptions& iPerformanceOptions )
{
    switch( iSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                return  ::ULIS::TMakeContext< ULIS_REG[ n ] >::GetTrimmedTransparencyRect( (::ULIS::TBlock< ULIS_REG[ n ] >*)iSrc           \
                                                                                         , iPerformanceOptions );                           \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
            default: return  FRect( 0, 0, iSrc->Width(), iSrc->Height() );
        #undef ULIS_REG_SWITCH_OP
    }
}



} // namespace ULIS

