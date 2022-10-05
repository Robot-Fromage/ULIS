// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterLine.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of Line
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include "Process/Raster/RasterUtils.h"

ULIS_NAMESPACE_BEGIN

void SimulateLine(
    FBlock& iBlock
    , const FVec2F& iP0
    , const FVec2F& iP1
    , TArray<FVec2F>& ioLinePoints );

void DrawLine(
      FBlock& iBlock
    , const FVec2I& iP0
    , const FVec2I& iP1
    , const FColor& iColor
    , const FRectI& iClippingRect);

template< typename T >
void DrawLineAA(      FBlock&     iBlock
                    , const FVec2I& iP0
                    , const FVec2I& iP1
                    , const FColor& iColor 
                    , const FRectI& iClippingRect )
{
    //Clipping ----
    FVec2I p0 = iP0;
    FVec2I p1 = iP1;
    
    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;
    
    if( clippingRect.Area() == 0 )
    {
        clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
    }
    
    int xMax = clippingRect.x + clippingRect.w;
    int yMax = clippingRect.y + clippingRect.h;
    int xMin = clippingRect.x;
    int yMin = clippingRect.y;
    
    int codeP0 = InternalComputeCodeForPoint( p0, clippingRect );
    int codeP1 = InternalComputeCodeForPoint( p1, clippingRect );
    
    bool accept = false;

    while (true)
    {
        if ((codeP0 == 0) && (codeP1 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (codeP0 & codeP1)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            int x, y;
  
            // At least one endpoint is outside the
            // rectangle, pick it.
            if (codeP0 != 0)
                code_out = codeP0;
            else
                code_out = codeP1;
  
            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & eClippingZone::kBot)
            {
                // point is above the clip rectangle
                x = p0.x + (p1.x - p0.x) * (yMax - p0.y) / (p1.y - p0.y);
                y = yMax;
            }
            else if (code_out & eClippingZone::kTop)
            {
                // point is below the rectangle
                x = p0.x + (p1.x - p0.x) * (yMin - p0.y) / (p1.y - p0.y);
                y = yMin;
            }
            else if (code_out & eClippingZone::kRight)
            {
                // point is to the right of rectangle
                y = p0.y + (p1.y - p0.y) * (xMax - p0.x) / (p1.x - p0.x);
                x = xMax;
            }
            else if (code_out & eClippingZone::kLeft)
            {
                // point is to the left of rectangle
                y = p0.y + (p1.y - p0.y) * (xMin - p0.x) / (p1.x - p0.x);
                x = xMin;
            }
  
            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == codeP0)
            {
                p0.x = x;
                p0.y = y;
                codeP0 = InternalComputeCodeForPoint(p0, clippingRect);
            }
            else
            {
                p1.x = x;
                p1.y = y;
                codeP1 = InternalComputeCodeForPoint(p1, clippingRect);
            }
        }
    }
    
    if( !accept )
        return; //Nothing to draw
    
    //Drawing
    FColor val = iColor;
    T maxAlpha = val.AlphaT<T>();

    if( ::ULIS::FMath::Abs( p1.y - p0.y ) < ::ULIS::FMath::Abs( p1.x - p0.x )) //x slope > y slope
    {
        if( p1.x < p0.x )
        {
            ULIS::FVec2I temp = p0;
            p0 = p1;
            p1 = temp;
        }

        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int yStep = 1;
        int limit = clippingRect.h;

        if( dy < 0 )
        {
            limit = clippingRect.y;
            yStep = -1;
            dy = -dy;
        }

        int errMax = -2 * dx + 2 * dy + 1;
        int errMin = 2 * dy - 1;
        int slopeDifferential = 2 * dy - dx;
        int y = p0.y;

        for( int x = p0.x; x <= p1.x; x++ )
        {
            float alphaTop = float(1 - FMath::Abs( ( float( slopeDifferential - errMax ) / float( errMin - errMax ) ) - 0.5 ) ); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>( T(maxAlpha * alphaTop) );
            iBlock.SetPixel( x, y, val );

            if( y != limit )
            {
                val.SetAlphaT<T>( T(maxAlpha * (1 - alphaTop)) );
                iBlock.SetPixel( x, y + yStep, val );
            }

            if( slopeDifferential >= dx )
            {
                y += yStep;
                slopeDifferential-=(2 * dx);
            }
            slopeDifferential+=(2 * dy);
        }
    }
    else //y slope > x slope
    {
        if( p1.y < p0.y )
        {
            ULIS::FVec2I temp = p0;
            p0 = p1;
            p1 = temp;
        }

        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int xStep = 1;
        int limit = clippingRect.w;

        if( dx < 0)
        {
            limit = clippingRect.x;
            xStep = -1;
            dx = -dx;
        }

        int errMax = -2 * dy + 2 * dx + 1;
        int errMin = 2 * dx - 1;
        int slopeDifferential = 2 * dx - dy;
        int x = p0.x;

        for( int y = p0.y; y <= p1.y; y++ )
        {
            float alphaTop = float(1 - FMath::Abs( ( float( slopeDifferential - errMax ) / float( errMin - errMax ) ) - 0.5 ) ); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>( T(maxAlpha * alphaTop) );
            iBlock.SetPixel( x, y, val );

            if( x != limit )
            {
                val.SetAlphaT<T>( T(maxAlpha * (1 - alphaTop )) );
                iBlock.SetPixel( x + xStep, y, val );
            }

            if( slopeDifferential >= dy )
            {
                x += xStep;
                slopeDifferential-=(2 * dy);
            }
            slopeDifferential+=(2 * dx);
        }
    }
}

template< typename T >
void DrawLineSP(      FBlock&     iBlock
                    , const FVec2F& iP0
                    , const FVec2F& iP1
                    , const FColor& iColor 
                    , const FRectI& iClippingRect )
{
    //Clipping ----
    FVec2F p0 = iP0;
    FVec2F p1 = iP1;
    
    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;
    
    if( clippingRect.Area() == 0 )
    {
        clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
    }
    
    int xMax = clippingRect.x + clippingRect.w;
    int yMax = clippingRect.y + clippingRect.h;
    int xMin = clippingRect.x;
    int yMin = clippingRect.y;
    
    int codeP0 = InternalComputeCodeForPoint( p0, clippingRect );
    int codeP1 = InternalComputeCodeForPoint( p1, clippingRect );
    
    bool accept = false;

    while (true)
    {
        if ((codeP0 == 0) && (codeP1 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (codeP0 & codeP1)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        }
        else
        {
            // Some segment of line lies within the
            // rectangle
            int code_out;
            int x, y;
  
            // At least one endpoint is outside the
            // rectangle, pick it.
            if (codeP0 != 0)
                code_out = codeP0;
            else
                code_out = codeP1;
  
            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if (code_out & eClippingZone::kBot)
            {
                // point is above the clip rectangle
                x = int(p0.x + (p1.x - p0.x) * (yMax - p0.y) / (p1.y - p0.y));
                y = yMax;
            }
            else if (code_out & eClippingZone::kTop)
            {
                // point is below the rectangle
                x = int(p0.x + (p1.x - p0.x) * (yMin - p0.y) / (p1.y - p0.y));
                y = yMin;
            }
            else if (code_out & eClippingZone::kRight)
            {
                // point is to the right of rectangle
                y = int(p0.y + (p1.y - p0.y) * (xMax - p0.x) / (p1.x - p0.x));
                x = xMax;
            }
            else if (code_out & eClippingZone::kLeft)
            {
                // point is to the left of rectangle
                y = int(p0.y + (p1.y - p0.y) * (xMin - p0.x) / (p1.x - p0.x));
                x = xMin;
            }
  
            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == codeP0)
            {
                p0.x = float(x);
                p0.y = float(y);
                codeP0 = InternalComputeCodeForPoint(p0, clippingRect);
            }
            else
            {
                p1.x = float(x);
                p1.y = float(y);
                codeP1 = InternalComputeCodeForPoint(p1, clippingRect);
            }
        }
    }
    
    if( !accept )
        return; //Nothing to draw
    
    //Drawing
    FColor val = iColor;
    T maxAlpha = val.AlphaT<T>();

    if( ::ULIS::FMath::Abs( p1.y - p0.y ) < ::ULIS::FMath::Abs( p1.x - p0.x )) //x slope > y slope
    {
        if( p1.x < p0.x )
        {
            ULIS::FVec2F temp = p0;
            p0 = p1;
            p1 = temp;
        }

        float dx = p1.x - p0.x;
        float dy = p1.y - p0.y;
        int yStep = 1;
        int limit = clippingRect.h;

        if( dy < 0)
        {
            limit = clippingRect.y;
            yStep = -1;
            dy = -dy;
        }

        float errMax = -2 * dx + 2 * dy + 1;
        float errMin = 2 * dy - 1;
        float slopeDifferential = 2 * dy - ( 2 * dx ) * InternalGetPixelBaseAlphaFromCoord( p0 );
        float y = p0.y;
        
        for( float x = p0.x; x <= p1.x; x++)
        {
            float alphaTop = float(1 - FMath::Abs( ( float( slopeDifferential - errMax ) / float( errMin - errMax ) ) ) ); //Interpolation of slopedifferential between errMin and errMax

            if (alphaTop <= 0)
            {
                y += yStep;
                slopeDifferential -= (2 * dx);
                alphaTop += 1;
            }

            val.SetAlphaT<T>( T(maxAlpha * alphaTop) );
            iBlock.SetPixel( int(x), int(y), val );

            if( int(y) != limit )
            {
                val.SetAlphaT<T>( T(maxAlpha * (1 - alphaTop)) );
                iBlock.SetPixel( int(x), int(y + yStep), val );
            }

            slopeDifferential+=(2 * dy);
        }
    }
    else //y slope > x slope
    {
        if( p1.y < p0.y )
        {
            ULIS::FVec2F temp = p0;
            p0 = p1;
            p1 = temp;
        }

        float dx = p1.x - p0.x;
        float dy = p1.y - p0.y;
        int xStep = 1;
        int limit = clippingRect.w;

        if( dx < 0)
        {
            limit = clippingRect.x;
            xStep = -1;
            dx = -dx;
        }

        float errMax = -2 * dy + 2 * dx + 1;
        float errMin = 2 * dx - 1;
        float slopeDifferential = 2 * dx - ( 2 * dy )*InternalGetPixelBaseAlphaFromCoord( p0 );
        float x = p0.x;

        for( float y = p0.y; y <= p1.y; y++)
        {
            float alphaTop = float(1 - FMath::Abs( ( float( slopeDifferential - errMax ) / float( errMin - errMax ) ) ) ); //Interpolation of slopedifferential between errMin and errMax

            if (alphaTop <= 0)
            {
                x += xStep;
                slopeDifferential -= (2 * dy);
                alphaTop+=1;
            }

            val.SetAlphaT<T>( T(maxAlpha * alphaTop) );
            iBlock.SetPixel( int(x), int(y), val );

            if( int(x) != limit )
            {
                val.SetAlphaT<T>( T(maxAlpha * (1 - alphaTop )) );
                iBlock.SetPixel( int(x + xStep), int(y), val );
            }

            slopeDifferential+=(2 * dx);
        }
    }
}

ULIS_NAMESPACE_END

