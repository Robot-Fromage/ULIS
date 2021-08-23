// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.Transform.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FTransformContext class.
 */
#include "ULIS/ULIS.Config.h"
#include "ULIS/Interface/ULIS.Interface.Transform.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Transform/ULIS.Transform.TransformContext.h"
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>

namespace ULIS {
/////////////////////////////////////////////////////
// FTransformContext
//--------------------------------------------------------------------------------------
//------------------------------------------------------------- Public Static Matrix API
//static
glm::mat3
FTransformContext::GetIdentityMatrix()
{
    return  glm::identity< glm::mat3 >();
}


//static
glm::mat3
FTransformContext::GetTranslationMatrix( float iX, float iY )
{
    return  glm::translate( GetIdentityMatrix(), glm::vec2( iX, iY ) );
}


//static
glm::mat3
FTransformContext::GetRotationMatrix( float iAngleRad )
{
    return  glm::rotate( GetIdentityMatrix(), iAngleRad );
}


//static
glm::mat3
FTransformContext::GetScaleMatrix( float iX, float iY )
{
    return  glm::scale( GetIdentityMatrix(), glm::vec2( iX, iY ) );
}


//static
glm::mat3
FTransformContext::GetShearMatrix( float iX, float iY )
{
    return  glm::shearY( glm::shearX( GetIdentityMatrix(), iX ), iY );
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
IBlock*
FTransformContext::GetTransformed( const IBlock* iBlockSrc
                                 , const  glm::mat3& iMat
                                 , eResamplingMethod iResamplingMethod
                                 , const FPerformanceOptions& iPerformanceOptions )
{
    switch( iBlockSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                return  TTransformContext::GetTransformedBlock< ULIS_REG[ n ] >( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockSrc                \
                                                                               , iMat, iResamplingMethod, iPerformanceOptions );            \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }

    return  nullptr;
}


//static
IBlock*
FTransformContext::TransformInto( const IBlock* iBlockSrc
                                , IBlock* iBlockDst
                                , const  glm::mat3& iMat
                                , eResamplingMethod iResamplingMethod
                                , const FPerformanceOptions& iPerformanceOptions )
{
    switch( iBlockSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                TTransformContext::TransformBlockInto< ULIS_REG[ n ] >( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockSrc                         \
                                                                      , (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockDst                         \
                                                                      , iMat, iResamplingMethod, iPerformanceOptions );                     \
                break;                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }

    return  nullptr;
}


//static
FRect
FTransformContext::GetTransformPreviewRect( const IBlock* iBlockSrc
                                          , const  glm::mat3& iMat )
{
    FTransformAABB aabb( 0, 0, iBlockSrc->Width(), iBlockSrc->Height() );
    aabb.Transform( iMat );
    FRect result( aabb.x1, aabb.x2, aabb.Width(), aabb.Height() );
    return  result;
}


} // namespace ULIS

