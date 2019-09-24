/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.Interface.Make.cpp
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Make/ULIS.Make.MakeContext.h"
#include "ULIS/Interface/ULIS.Interface.Make.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FMakeContext

//static
::ULIS::IBlock*
FMakeContext::MakeBlock( int width, int height, uint32_t ID, const std::string& iProfileTag )
{
    switch( ID )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ULIS_REG[ n ]: return  new ::ULIS::TBlock< ULIS_REG[ n ] >( width, height, iProfileTag );
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
        default: return  nullptr;
    }
}

//static
::ULIS::IBlock*
FMakeContext::MakeBlockFromExternalData( int width, int height, uint8* iData, uint32_t ID, const std::string& iProfileTag )
{
    switch( ID )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ULIS_REG[ n ]: return  new ::ULIS::TBlock< ULIS_REG[ n ] >( width, height, iData, iProfileTag );
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
        default: return  nullptr;
    }
}


//static
::ULIS::IBlock*
FMakeContext::MakeBlockFromDataPerformCopy( int width, int height, uint8* iData, uint32_t ID, const std::string& iProfileTag, const FPerformanceOptions& iPerformanceOptions)
{
    ::ULIS::IBlock* src = MakeBlockFromExternalData( width, height, iData, ID, iProfileTag );
    ::ULIS::IBlock* ret = MakeBlock( width, height, ID, iProfileTag );
    CopyBlockInto( src, ret );
    delete src;
    return  ret;
}


//static
::ULIS::IBlock*
FMakeContext::CopyBlock( ::ULIS::IBlock* iBlock, const FPerformanceOptions& iPerformanceOptions)
{
    return  CopyBlockRect( iBlock, FRect( 0, 0, iBlock->Width(), iBlock->Height() ), iPerformanceOptions);
}


//static
void
FMakeContext::CopyBlockInto( ::ULIS::IBlock* iSrc, ::ULIS::IBlock* iDst, const FPerformanceOptions& iPerformanceOptions)
{
    CopyBlockRectInto( iSrc, iDst, FRect( 0, 0, iDst->Width(), iDst->Height() ), iPerformanceOptions);
}


//static
::ULIS::IBlock*
FMakeContext::CopyBlockRect( ::ULIS::IBlock* iBlock, const FRect& iRect, const FPerformanceOptions& iPerformanceOptions)
{
    ::ULIS::IBlock* ret = MakeBlock( iRect.w, iRect.h, iBlock->Id() );
    CopyBlockRectInto( iBlock, ret, iRect, iPerformanceOptions);
    return  ret;
}


//static
void
FMakeContext::CopyBlockRectInto( ::ULIS::IBlock* iSrc, ::ULIS::IBlock* iDst, const FRect& iRect, const FPerformanceOptions& iPerformanceOptions)
{
    assert( iSrc->Id() == iDst->Id() );

    switch( iSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                    \
            case ULIS_REG[ n ]:                                                       \
            {                                                                                       \
                        ::ULIS::TMakeContext< ULIS_REG[ n ] >                         \
                        ::CopyBlockRectInto( (::ULIS::TBlock< ULIS_REG[ n ] >*)iSrc   \
                                           , (::ULIS::TBlock< ULIS_REG[ n ] >*)iDst   \
                                           , iRect, iPerformanceOptions);                                   \
                        break;                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
    if( iSrc->ColorProfile() )
        iDst->AssignColorProfile( iSrc->ColorProfile()->Name() );
}

} // namespace ULIS

