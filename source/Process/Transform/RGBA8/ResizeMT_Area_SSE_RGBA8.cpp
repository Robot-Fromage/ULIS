// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ResizeMT_Area_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Transform/RGBA8/ResizeMT_Area_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeResizeMT_Area_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FResizeCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    //const FFormatMetrics& sat_fmt = cargs->optionalSAT->FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;

    FVec2F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line );
    FVec2F point_in_src( cargs->inverseScale * ( point_in_dst - cargs->shift ) + FVec2F( cargs->srcRect.x, cargs->srcRect.y ) );
    FVec2F src_dx( cargs->inverseScale * FVec2F( 1.f, 0.f ) );
    FVec2F coverage( FVec2F( 1.f, 1.f ) * cargs->inverseScale );
    Vec4f coverage_area = coverage.x * coverage.y;

    const int minx = cargs->srcRect.x;
    const int miny = cargs->srcRect.y;
    const int maxx = minx + cargs->srcRect.w;
    const int maxy = miny + cargs->srcRect.h;

    Vec4f c00, c10, c11, c01, hh0, hh1, res, alp;
    Vec4f m00, m10, m11, m01;

    float fpos[4];
    int   ipos[4];
    Vec4f t[4];
    Vec4f u[4];

    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        // order: left top right bot
        fpos[0] = point_in_src.x - 1.f;
        fpos[1] = point_in_src.y - 1.f;
        fpos[2] = fpos[0] + coverage.x;
        fpos[3] = fpos[1]  + coverage.y;
        for( int i = 0; i < 4; ++i ) {
            ipos[i] = static_cast< int >( fpos[i] );
            t[i]    = fpos[i] - ipos[i];
            u[i]    = 1.f - t[i];
        }

        #define LOAD( X )   _mm_loadu_ps( reinterpret_cast< const float* >( X ) )
        #define SUBSAMPLE_CORNER_IMP( _C, _X, _Y )                                                                                                          \
            if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) {                                                                                      \
                const uint8* pptr = cargs->optionalSAT->PixelBits( _X, _Y );                                                                                   \
                _C = LOAD( pptr );                                                                                                                          \
            } else {                                                                                                                                        \
                _C = _mm_setzero_ps();                                                                                                                      \
            }
        #define SUBSAMPLE_CORNER( _P0, _P1, _M )                                                            \
            SUBSAMPLE_CORNER_IMP( c00, ipos[ _P0 ],     ipos[ _P1 ]     );                                  \
            SUBSAMPLE_CORNER_IMP( c10, ipos[ _P0 ] + 1, ipos[ _P1 ]     );                                  \
            SUBSAMPLE_CORNER_IMP( c11, ipos[ _P0 ] + 1, ipos[ _P1 ] + 1 );                                  \
            SUBSAMPLE_CORNER_IMP( c01, ipos[ _P0 ],     ipos[ _P1 ] + 1 );                                  \
            hh0 = c00 * u[ _P0 ] + c10 * t[ _P0 ];                                                          \
            hh1 = c01 * u[ _P0 ] + c11 * t[ _P0 ];                                                          \
            _M  = hh0 * u[ _P1 ] + hh1 * t[ _P1 ];
        SUBSAMPLE_CORNER( 0, 1, m00 )
        SUBSAMPLE_CORNER( 2, 1, m10 )
        SUBSAMPLE_CORNER( 2, 3, m11 )
        SUBSAMPLE_CORNER( 0, 3, m01 )
        #undef SUBSAMPLE_CORNER_IMP
        #undef SUBSAMPLE_CORNER
        #undef LOAD

        res = ( m11 + m00 - m10 - m01 ) / coverage_area;
        alp = lookup4( fmt.AID, res );
        alp.insert( fmt.AID, 255.f );
        res = ( res * 255.f ) / alp;
        auto _pack = _mm_cvtps_epi32( res );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )dst = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

ULIS_DEFINE_RESIZE_COMMAND_SPECIALIZATION( ResizeMT_Area_SSE_RGBA8 )

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

