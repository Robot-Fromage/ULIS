// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineTiledMT_Bicubic_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Transform/RGBA8/TransformAffineTiledMT_Bicubic_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformAffineTiledMT_Bicubic_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    //Vec4i _idt( fmt.IDT[0], fmt.IDT[1], fmt.IDT[2], fmt.IDT[3] );
    Vec4i _idt( 0, 1, 2, 3 );
    _idt.insert( fmt.AID, 4 );

    FVec3F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line, 1.f );
    FVec2F point_in_src( cargs->inverseMatrix * point_in_dst );
    FVec2F src_dx( cargs->inverseMatrix * FVec3F( 1.f, 0.f, 0.f ) );

    Vec4f p00, p10, p20, p30;
    Vec4f p01, p11, p21, p31;
    Vec4f p02, p12, p22, p32;
    Vec4f p03, p13, p23, p33;
    Vec4f hh0, hh1, hh2, hh3;
    Vec4f res, alp;

    const int minx = cargs->srcRect.x;
    const int miny = cargs->srcRect.y;
    const int maxx = minx + cargs->srcRect.w;
    const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const int   src_x   = static_cast< int >( floor( point_in_src.x ) );
        const int   src_y   = static_cast< int >( floor( point_in_src.y ) );
        const Vec4f tx      = point_in_src.x - src_x;
        const Vec4f ty      = point_in_src.y - src_y;

        const int xm1 = FMath::PyModulo( src_x - 1, cargs->srcRect.w );
        const int xp0 = FMath::PyModulo( src_x    , cargs->srcRect.w );
        const int xp1 = FMath::PyModulo( src_x + 1, cargs->srcRect.w );
        const int xp2 = FMath::PyModulo( src_x + 2, cargs->srcRect.w );
        const int ym1 = FMath::PyModulo( src_y - 1, cargs->srcRect.h );
        const int yp0 = FMath::PyModulo( src_y    , cargs->srcRect.h );
        const int yp1 = FMath::PyModulo( src_y + 1, cargs->srcRect.h );
        const int yp2 = FMath::PyModulo( src_y + 2, cargs->srcRect.h );
        #define LOAD( X )   _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( X ) ) ) )
        #define GETPIXEL( _C, _X, _Y )                                                                                                                      \
            if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) {                                                                                      \
                const uint8* pptr = cargs->src.PixelBits( _X, _Y );                                                                                        \
                Vec4f _ch = LOAD( pptr );                                                                                                                   \
                Vec4f _al = _mm_set_ps1( pptr[ fmt.AID ] );                                                                                                 \
                _C = lookup8( _idt, ( _ch * _al ) / 255.f, _al );                                                                                           \
            } else {                                                                                                                                        \
                _C = _mm_setzero_ps();                                                                                                                      \
            }
        GETPIXEL( p00, xm1,     ym1 );  GETPIXEL( p01, xm1,     yp0 );  GETPIXEL( p02, xm1,     yp1 );  GETPIXEL( p03, xm1,     yp2 );
        GETPIXEL( p10, xp0,     ym1 );  GETPIXEL( p11, xp0,     yp0 );  GETPIXEL( p12, xp0,     yp1 );  GETPIXEL( p13, xp0,     yp2 );
        GETPIXEL( p20, xp1,     ym1 );  GETPIXEL( p21, xp1,     yp0 );  GETPIXEL( p22, xp1,     yp1 );  GETPIXEL( p23, xp1,     yp2 );
        GETPIXEL( p30, xp2,     ym1 );  GETPIXEL( p31, xp2,     yp0 );  GETPIXEL( p32, xp2,     yp1 );  GETPIXEL( p33, xp2,     yp2 );
        #undef GETPIXEL
        #undef LOAD

        hh0 = InterpCubic( p00, p10, p20, p30, tx );
        hh1 = InterpCubic( p01, p11, p21, p31, tx );
        hh2 = InterpCubic( p02, p12, p22, p32, tx );
        hh3 = InterpCubic( p03, p13, p23, p33, tx );
        res = InterpCubic( hh0, hh1, hh2, hh3, ty );
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

ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( TransformAffineTiledMT_Bicubic_SSE_RGBA8 )

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

