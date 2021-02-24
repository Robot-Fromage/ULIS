// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterAlgorithms.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions and structs used in the Raster API
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
namespace eClippingZone
{
const uint8 kInside   = 0b0000;
const uint8 kLeft     = 0b0001;
const uint8 kRight    = 0b0010;
const uint8 kBot      = 0b0100;
const uint8 kTop      = 0b1000;
};
typedef uint8 tClippingCode;

static const int InternalComputeCodeForPoint (const FVec2I& iPoint,const FRectI& iClippingRect)
{
    // initialized as being inside
    int code = eClippingZone::kInside;

    if(iPoint.x < iClippingRect.x)       // to the left of rectangle
        code |= eClippingZone::kLeft;
    else if(iPoint.x > (iClippingRect.x + iClippingRect.w))  // to the right of rectangle
        code |= eClippingZone::kRight;
    if(iPoint.y < iClippingRect.y)       // above the rectangle
        code |= eClippingZone::kTop;
    else if(iPoint.y > (iClippingRect.y + iClippingRect.h))  // below the rectangle
        code |= eClippingZone::kBot;

    return code;
}

// Returns true if ioP0 and ioP1 are different at the end of the algorithm
static const bool InternalCropLineToRectangle(FVec2I& ioP0,FVec2I& ioP1,const FRectI& iClippingRect)
{
    int xMax = iClippingRect.x + iClippingRect.w;
    int yMax = iClippingRect.y + iClippingRect.h;
    int xMin = iClippingRect.x;
    int yMin = iClippingRect.y;

    tClippingCode codeP0 = InternalComputeCodeForPoint(ioP0,iClippingRect);
    tClippingCode codeP1 = InternalComputeCodeForPoint(ioP1,iClippingRect);
    bool accept = false;

    while(true)
    {
        if((codeP0 == 0) && (codeP1 == 0))
        {
            // If both endpoints lie within rectangle
            accept = true;
            break;
        } 
        else if(codeP0 & codeP1)
        {
            // If both endpoints are outside rectangle,
            // in same region
            break;
        } 
        else
        {
            // Some segment of line lies within the
            // rectangle
            tClippingCode code_out;
            int x,y;

            // At least one endpoint is outside the
            // rectangle, pick it.
            if(codeP0 != 0)
                code_out = codeP0;
            else
                code_out = codeP1;

            // Find intersection point;
            // using formulas y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)
            if(code_out & eClippingZone::kBot)
            {
                // point is above the clip rectangle
                x = ioP0.x + (ioP1.x - ioP0.x) * (yMax - ioP0.y) / (ioP1.y - ioP0.y);
                y = yMax;
            } 
            else if(code_out & eClippingZone::kTop)
            {
                // point is below the rectangle
                x = ioP0.x + (ioP1.x - ioP0.x) * (yMin - ioP0.y) / (ioP1.y - ioP0.y);
                y = yMin;
            } 
            else if(code_out & eClippingZone::kRight)
            {
                // point is to the right of rectangle
                y = ioP0.y + (ioP1.y - ioP0.y) * (xMax - ioP0.x) / (ioP1.x - ioP0.x);
                x = xMax;
            } 
            else if(code_out & eClippingZone::kLeft)
            {
                // point is to the left of rectangle
                y = ioP0.y + (ioP1.y - ioP0.y) * (xMin - ioP0.x) / (ioP1.x - ioP0.x);
                x = xMin;
            }

            // Now intersection point x,y is found
            // We replace point outside rectangle
            // by intersection point
            if(code_out == codeP0)
            {
                ioP0.x = x;
                ioP0.y = y;
                codeP0 = InternalComputeCodeForPoint(ioP0,iClippingRect);
            } 
            else
            {
                ioP1.x = x;
                ioP1.y = y;
                codeP1 = InternalComputeCodeForPoint(ioP1,iClippingRect);
            }
        }
    }
    return accept;
}


static void DrawLine( FBlock& iBlock
                    , const FVec2I& iP0
                    , const FVec2I& iP1
                    , const FColor& iColor 
                    , const FRectI& iClippingRect )
{
    //Clipping ----
    
    FVec2I p0 = iP0;
    FVec2I p1 = iP1;
    
    FRectI clippingRect = iClippingRect;
    
    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0, iBlock.Width() - 1, iBlock.Height() - 1);
    }

    if(!InternalCropLineToRectangle(p0,p1,clippingRect))
        return; //Nothing to draw

    //Drawing ----
    FColor val = iColor;
    
    if(::ULIS::FMath::Abs(p1.y - p0.y) < ::ULIS::FMath::Abs(p1.x - p0.x)) // x slope > y slope
    {
        if(p1.x < p0.x)
        {
            FVec2I temp = p0;
            p0 = p1;
            p1 = temp;
        }

        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int yStep = 1;

        if(dy < 0)
        {
            yStep = -1;
            dy = -dy;
        }

        int slopeDifferential = 2 * dy - dx;
        int y = p0.y;

        for(int x = p0.x; x <= p1.x; x++)
        {
            iBlock.SetPixel( x, y, val );

            if(slopeDifferential > 0)
            {
                y += yStep;
                slopeDifferential-=(2 * dx);
            }
            slopeDifferential+=(2 * dy);
        }
    } 
    else //y slope > x slope
    {
        if(p1.y < p0.y)
        {
            FVec2I temp = p0;
            p0 = p1;
            p1 = temp;
        }

        int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int xStep = 1;

        if(dx < 0)
        {
            xStep = -1;
            dx = -dx;
        }

        int slopeDifferential = 2 * dx - dy;
        int x = p0.x;

        for(int y = p0.y; y <= p1.y; y++)
        {
            iBlock.SetPixel(x,y,val);

            if(slopeDifferential > 0)
            {
                x += xStep;
                slopeDifferential-=(2 * dy);
            }
            slopeDifferential+=(2 * dx);
        }
    }
}

static void DrawLineAA( FBlock& iBlock
                    , const FVec2I& iP0
                    , const FVec2I& iP1
                    , const FColor& iColor 
                    , const FRectI& iClippingRect )
{
    //Clipping ----
    
    FVec2I p0 = iP0;
    FVec2I p1 = iP1;
    
    FRectI clippingRect = iClippingRect;
    
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
    uint8 maxAlpha = val.Alpha8();

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

        if( dy < 0)
        {
            yStep = -1;
            dy = -dy;
        }

        int errMax = -2 * dx + 2 * dy + 1;
        int errMin = 2 * dy - 1;
        int slopeDifferential = 2 * dy - dx;
        int y = p0.y;

        for( int x = p0.x; x <= p1.x; x++)
        {
            float alphaTop = (1 - FMath::Abs( ( float( slopeDifferential - errMax ) / float( errMin - errMax ) ) - 0.5 ) ); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlpha8( maxAlpha * alphaTop );
            iBlock.SetPixel( x, y, val );

            val.SetAlpha8( maxAlpha * (1 - alphaTop ) );
            iBlock.SetPixel( x, y + yStep, val );

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

        if( dx < 0)
        {
            xStep = -1;
            dx = -dx;
        }

        int errMax = -2 * dy + 2 * dx + 1;
        int errMin = 2 * dx - 1;
        int slopeDifferential = 2 * dx - dy;
        int x = p0.x;

        for( int y = p0.y; y <= p1.y; y++)
        {
            float alphaTop = (1 - FMath::Abs( ( float( slopeDifferential - errMax ) / float( errMin - errMax ) ) - 0.5 ) ); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlpha8( maxAlpha * alphaTop );
            iBlock.SetPixel( x, y, val );

            val.SetAlpha8( maxAlpha * (1 - alphaTop ) );
            iBlock.SetPixel( x + xStep, y, val );

            if( slopeDifferential >= dy )
            {
                x += xStep;
                slopeDifferential-=(2 * dy);
            }
            slopeDifferential+=(2 * dx);
        }
    }
}


static void DrawCircleAndres(     FBlock& iBlock
                                , const FVec2I             iCenter
                                , const int                iRadius
                                , const FColor&            iColor
                                , const bool               iFilled
                                , const FRectI&            iClippingRect )
{
    //Clipping -----
    int x = 0;
    int y = iRadius; //We start from the top of the circle for the first octant

    FRectI clippingRect = iClippingRect;
    
    if( clippingRect.Area() == 0 )
    {
        clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
    }
    
    int shift45 = std::sin( FMath::DegToRad( 45 ) ) * iRadius;
    FVec2I point0 = FVec2I( iCenter.x, iCenter.y - iRadius );
    FVec2I point45 = FVec2I( iCenter.x + shift45, iCenter.y - shift45 );
    FVec2I point90 = FVec2I( iCenter.x + iRadius, iCenter.y );
    FVec2I point135 = FVec2I( iCenter.x + shift45, iCenter.y + shift45 );
    FVec2I point180 = FVec2I( iCenter.x, iCenter.y + iRadius );
    FVec2I point225 = FVec2I( iCenter.x - shift45, iCenter.y + shift45 );
    FVec2I point270 = FVec2I( iCenter.x - iRadius, iCenter.y );
    FVec2I point315 = FVec2I( iCenter.x - shift45, iCenter.y - shift45 );
    
    FRectI rectOctant1 = FRectI( point0.x, point0.y, point45.x - point0.x, point45.y - point0.y );
    FRectI rectOctant2 = FRectI( point45.x, point45.y, point90.x - point45.x, point90.y - point45.y );
    FRectI rectOctant3 = FRectI( point135.x, point90.y, point90.x - point135.x, point135.y - point90.y );
    FRectI rectOctant4 = FRectI( point180.x, point135.y, point135.x - point180.x, point180.y - point135.y );
    FRectI rectOctant5 = FRectI( point225.x, point225.y, point180.x - point225.x, point180.y - point225.y );
    FRectI rectOctant6 = FRectI( point270.x, point270.y, point225.x - point270.x, point225.y - point270.y );
    FRectI rectOctant7 = FRectI( point270.x, point315.y, point315.x - point270.x, point270.y - point315.y );
    FRectI rectOctant8 = FRectI( point315.x, point0.y, point0.x - point315.x, point315.y - point0.y );
    
    FRectI rectOctant1Clipped = rectOctant1 & clippingRect;
    FRectI rectOctant2Clipped = rectOctant2 & clippingRect;
    FRectI rectOctant3Clipped = rectOctant3 & clippingRect;
    FRectI rectOctant4Clipped = rectOctant4 & clippingRect;
    FRectI rectOctant5Clipped = rectOctant5 & clippingRect;
    FRectI rectOctant6Clipped = rectOctant6 & clippingRect;
    FRectI rectOctant7Clipped = rectOctant7 & clippingRect;
    FRectI rectOctant8Clipped = rectOctant8 & clippingRect;

    // 0 : Don't draw the octant. 1: Draw fully the octant. 2: Partial draw of the octant (complex case)
    int drawRectOctant1 = rectOctant1Clipped == rectOctant1 ? 1 : rectOctant1Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant2 = rectOctant2Clipped == rectOctant2 ? 1 : rectOctant2Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant3 = rectOctant3Clipped == rectOctant3 ? 1 : rectOctant3Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant4 = rectOctant4Clipped == rectOctant4 ? 1 : rectOctant4Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant5 = rectOctant5Clipped == rectOctant5 ? 1 : rectOctant5Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant6 = rectOctant6Clipped == rectOctant6 ? 1 : rectOctant6Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant7 = rectOctant7Clipped == rectOctant7 ? 1 : rectOctant7Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant8 = rectOctant8Clipped == rectOctant8 ? 1 : rectOctant8Clipped.Area() != 0 ? 2 : 0;
    
    //Drawing -----
    FColor val = iColor;
    
    
    //0° is on top and we turn clockwise
    //Octant 1 ------
    if( drawRectOctant1 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x + x, iCenter.y - y, val ); // 0° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant1 == 2 )
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;
        
        int diff = iRadius - 1;

        //Left and top clip
        while ( xx < rectOctant1Clipped.x || yy < rectOctant1Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx++;
            }
        }
        
        //Right and bottom clip
        while (xx <= limitX && yy <= limitY )
        {
            iBlock.SetPixel( iCenter.x + x, iCenter.y - y, val ); // 0° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx++;
            }
        }
    }
    
    
    //Octant 2 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant2 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x + y, iCenter.y - x, val ); // 90° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant2 == 2 )
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;
        
        int diff = iRadius - 1;

        //Right and bottom clip
        while ( xx > rectOctant2Clipped.x + rectOctant2Clipped.w || yy > rectOctant2Clipped.y + rectOctant2Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy--;
            }
        }
        
        //Top and left clip
        while (xx >= limitX && yy >= limitY )
        {
            iBlock.SetPixel( iCenter.x + y, iCenter.y - x, val ); // 90° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy--;
            }
        }
    }
    
    
    //Octant 3 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant3 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x + y, iCenter.y + x, val ); // 90° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant3 == 2 )
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;
        
        int diff = iRadius - 1;

        //Right and top clip
        while ( xx > rectOctant3Clipped.x + rectOctant3Clipped.w || yy < rectOctant3Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy++;
            }
        }
        
        //Bottom and left clip
        while (xx >= limitX && yy <= limitY )
        {
            iBlock.SetPixel( iCenter.x + y, iCenter.y + x, val ); // 90° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy++;
            }
        }
    }

    
    //Octant 4 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant4 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x + x, iCenter.y + y, val ); // 180° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant4 == 2 )
    {
        int xx = rectOctant4.x;
        int yy = rectOctant4.y + rectOctant4.h;
        int limitX = rectOctant4Clipped.x + rectOctant4Clipped.w;
        int limitY = rectOctant4Clipped.y;
        
        int diff = iRadius - 1;

        //Left and bottom clip
        while ( xx < rectOctant4Clipped.x || yy > rectOctant4Clipped.y + rectOctant4Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx++;
            }
        }
        
        //Bottom and left clip
        while (xx <= limitX && yy >= limitY )
        {
            iBlock.SetPixel( iCenter.x + x, iCenter.y + y, val ); // 180° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx++;
            }
        }
    }
    
    
    //Octant 5 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant5 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x - x, iCenter.y + y, val ); // 180° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant5 == 2 )
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;
        
        int diff = iRadius - 1;

        //Left and bottom clip
        while ( xx > rectOctant5Clipped.x + rectOctant5Clipped.w || yy > rectOctant5Clipped.y + rectOctant5Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx--;
            }
        }
        
        //Bottom and left clip
        while (xx >= limitX && yy >= limitY )
        {
            iBlock.SetPixel( iCenter.x - x, iCenter.y + y, val ); // 180° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx--;
            }
        }
    }
    
    
    //Octant 6 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant6 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x - y, iCenter.y + x, val );  // 270° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant6 == 2 )
    {
        int xx = rectOctant6.x;
        int yy = rectOctant6.y;
        int limitX = rectOctant6Clipped.x + rectOctant6Clipped.w;
        int limitY = rectOctant6Clipped.y + rectOctant6Clipped.h;
        
        int diff = iRadius - 1;

        //Left and bottom clip
        while ( xx < rectOctant6Clipped.x || yy < rectOctant6Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy++;
            }
        }
        
        //Bottom and left clip
        while (xx <= limitX && yy <= limitY )
        {
            iBlock.SetPixel( iCenter.x - y, iCenter.y + x, val );  // 270° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy++;
            }
        }
    }

    
    //Octant 7 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant7 == 1 )
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x - y, iCenter.y - x, val ); // 270° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant7 == 2 )
    {
        int xx = rectOctant7.x;
        int yy = rectOctant7.y + rectOctant7.h;
        int limitX = rectOctant7Clipped.x + rectOctant7Clipped.w;
        int limitY = rectOctant7Clipped.y;
        
        int diff = iRadius - 1;

        //Left and bottom clip
        while ( xx < rectOctant7Clipped.x || yy > rectOctant7Clipped.y + rectOctant7Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy--;
            }
        }
        
        //Bottom and left clip
        while (xx <= limitX && yy >= limitY )
        {
            iBlock.SetPixel( iCenter.x - y, iCenter.y - x, val ); // 270° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy--;
            }
        }
    }
    
    
    //Octant 8 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant8 == 1)
    {
        int diff = iRadius - 1;

        while (y >= x)
        {
            iBlock.SetPixel( iCenter.x - x, iCenter.y - y, val ); // 0° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant8 == 2 )
    {
        int xx = rectOctant8.x + rectOctant8.w;
        int yy = rectOctant8.y;
        int limitX = rectOctant8Clipped.x;
        int limitY = rectOctant8Clipped.y + rectOctant8Clipped.h;
        
        int diff = iRadius - 1;

        //Left and bottom clip
        while ( xx > rectOctant8Clipped.x + rectOctant8Clipped.w || yy < rectOctant8Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx--;
            }
        }
        
        //Bottom and left clip
        while (xx >= limitX && yy <= limitY )
        {
            iBlock.SetPixel( iCenter.x - x, iCenter.y - y, val ); // 0° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx--;
            }
        }
    }
    
    //Fill
    
    if( iFilled )
    {
        x = 0;
        y = iRadius;
        int diff = iRadius - 1;

        while (y >= x)
        {
            if( diff >= ( 2 * x ) )
            {
                DrawLine( iBlock, FVec2I( iCenter.x + x, iCenter.y - y ), FVec2I( iCenter.x + x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - x, iCenter.y - y ), FVec2I( iCenter.x - x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x + y, iCenter.y - x ), FVec2I( iCenter.x + y, iCenter.y + x ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - y, iCenter.y - x ), FVec2I( iCenter.x - y, iCenter.y + x ), iColor, iClippingRect );
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                DrawLine( iBlock, FVec2I( iCenter.x + x, iCenter.y - y ), FVec2I( iCenter.x + x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - x, iCenter.y - y ), FVec2I( iCenter.x - x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x + y, iCenter.y - x ), FVec2I( iCenter.x + y, iCenter.y + x ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - y, iCenter.y - x ), FVec2I( iCenter.x - y, iCenter.y + x ), iColor, iClippingRect );
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
}

static void DrawCircleAndresAA(   FBlock& iBlock
                                , const FVec2I             iCenter
                                , const int                iRadius
                                , const FColor&            iColor
                                , const bool               iFilled
                                , const FRectI&            iClippingRect )
{
        //Clipping -----
    int x = 0;
    int y = iRadius; //We start from the top of the circle for the first octant
    
    FRectI clippingRect = iClippingRect;
    
    if( clippingRect.Area() == 0 )
    {
        clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
    }
    
    int shift45 = std::sin( FMath::DegToRad( 45 ) ) * iRadius;
    FVec2I point0 = FVec2I( iCenter.x, iCenter.y - iRadius );
    FVec2I point45 = FVec2I( iCenter.x + shift45, iCenter.y - shift45 );
    FVec2I point90 = FVec2I( iCenter.x + iRadius, iCenter.y );
    FVec2I point135 = FVec2I( iCenter.x + shift45, iCenter.y + shift45 );
    FVec2I point180 = FVec2I( iCenter.x, iCenter.y + iRadius );
    FVec2I point225 = FVec2I( iCenter.x - shift45, iCenter.y + shift45 );
    FVec2I point270 = FVec2I( iCenter.x - iRadius, iCenter.y );
    FVec2I point315 = FVec2I( iCenter.x - shift45, iCenter.y - shift45 );
    
    FRectI rectOctant1 = FRectI( point0.x, point0.y - 1, point45.x - point0.x, point45.y - point0.y );
    FRectI rectOctant2 = FRectI( point45.x + 1, point45.y, point90.x - point45.x, point90.y - point45.y );
    FRectI rectOctant3 = FRectI( point135.x + 1, point90.y, point90.x - point135.x, point135.y - point90.y );
    FRectI rectOctant4 = FRectI( point180.x, point135.y + 1, point135.x - point180.x, point180.y - point135.y );
    FRectI rectOctant5 = FRectI( point225.x, point225.y + 1, point180.x - point225.x, point180.y - point225.y );
    FRectI rectOctant6 = FRectI( point270.x - 1, point270.y, point225.x - point270.x, point225.y - point270.y );
    FRectI rectOctant7 = FRectI( point270.x - 1, point315.y, point315.x - point270.x, point270.y - point315.y );
    FRectI rectOctant8 = FRectI( point315.x, point0.y - 1, point0.x - point315.x, point315.y - point0.y );
    
    FRectI rectOctant1Clipped = rectOctant1 & clippingRect;
    FRectI rectOctant2Clipped = rectOctant2 & clippingRect;
    FRectI rectOctant3Clipped = rectOctant3 & clippingRect;
    FRectI rectOctant4Clipped = rectOctant4 & clippingRect;
    FRectI rectOctant5Clipped = rectOctant5 & clippingRect;
    FRectI rectOctant6Clipped = rectOctant6 & clippingRect;
    FRectI rectOctant7Clipped = rectOctant7 & clippingRect;
    FRectI rectOctant8Clipped = rectOctant8 & clippingRect;

    // 0 : Don't draw the octant. 1: Draw fully the octant. 2: Partial draw of the octant (complex case)
    int drawRectOctant1 = rectOctant1Clipped == rectOctant1 ? 1 : rectOctant1Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant2 = rectOctant2Clipped == rectOctant2 ? 1 : rectOctant2Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant3 = rectOctant3Clipped == rectOctant3 ? 1 : rectOctant3Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant4 = rectOctant4Clipped == rectOctant4 ? 1 : rectOctant4Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant5 = rectOctant5Clipped == rectOctant5 ? 1 : rectOctant5Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant6 = rectOctant6Clipped == rectOctant6 ? 1 : rectOctant6Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant7 = rectOctant7Clipped == rectOctant7 ? 1 : rectOctant7Clipped.Area() != 0 ? 2 : 0;
    int drawRectOctant8 = rectOctant8Clipped == rectOctant8 ? 1 : rectOctant8Clipped.Area() != 0 ? 2 : 0;
    
    
    //Drawing ------
    FColor val = iColor;

    auto MaxAlpha = val.Alpha8();

    //0° is on top and we turn clockwise
    //Octant 1 ------
    if( drawRectOctant1 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y - y, val ); // 0° to 45°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y - y - 1, val ); // 0° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant1 == 2 )
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Left and top clip
        while ( xx < rectOctant1Clipped.x || yy < rectOctant1Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx++;
            }
        }
        
        // Base for drawing the aliased part of the circle on the sides on the clipping rect
        /*if( xx == rectOctant1Clipped.x || yy == rectOctant1Clipped.y ) //We draw only the bottom line of AA until we draw the next y
        {
            while ( 1 )
            {
                float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
                
                val.SetAlpha8( MaxAlpha * alphaTop );
                
                iBlock.SetPixel( iCenter.x + x, iCenter.y - y, val ); // 0° to 45°
                
                if( diff >= ( 2 * x ) )
                {
                    diff -= ( 2 * x + 1 );
                    x++; xx++;
                }
                else if ( diff < ( 2 * ( iRadius - y ) ) )
                {
                    diff += ( 2 * y - 1 );
                    y--; yy++;
                    break;
                }
                else
                {
                    diff += (2 * ( y - x - 1 ) );
                    y--; yy++;
                    x++; xx++;
                    break;
                }
            }
        }*/
        
        //Right and bottom clip
        while (xx <= limitX && yy <= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y - y, val ); // 0° to 45°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y - y - 1, val ); // 0° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx++;
            }
        }
    }
    
    //Octant 2 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant2 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + y, iCenter.y - x, val ); // 90° to 45°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + y + 1, iCenter.y - x, val ); // 90° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant2 == 2 )
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Right and bottom clip
        while ( xx > rectOctant2Clipped.x + rectOctant2Clipped.w || yy > rectOctant2Clipped.y + rectOctant2Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy--;
            }
        }
        
        //Top and left clip
        while (xx >= limitX && yy >= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + y, iCenter.y - x, val ); // 90° to 45°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + y + 1, iCenter.y - x, val ); // 90° to 45°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy--;
            }
        }
    }
    
    //Octant 3 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant3 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + y, iCenter.y + x, val ); // 90° to 135°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + y + 1, iCenter.y + x, val ); // 90° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant3 == 2 )
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Right and top clip
        while ( xx > rectOctant3Clipped.x + rectOctant3Clipped.w || yy < rectOctant3Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy++;
            }
        }
        
        //Top and left clip
        while (xx >= limitX && yy <= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + y, iCenter.y + x, val ); // 90° to 135°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + y + 1, iCenter.y + x, val ); // 90° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx--;
                x++; yy++;
            }
        }
    }
    
    //Octant 4 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant4 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y + y, val ); // 180° to 135°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y + y + 1, val ); // 180° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant4 == 2 )
    {
        int xx = rectOctant4.x;
        int yy = rectOctant4.y + rectOctant4.h;
        int limitX = rectOctant4Clipped.x + rectOctant4Clipped.w;
        int limitY = rectOctant4Clipped.y;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Right and top clip
        while ( xx < rectOctant4Clipped.x || yy > rectOctant4Clipped.y + rectOctant4Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx++;
            }
        }
        
        //Bottom and left clip
        while (xx <= limitX && yy >= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y + y, val ); // 180° to 135°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x + x, iCenter.y + y + 1, val ); // 180° to 135°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx++;
            }
        }
    }
    
    //Octant 5 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant5 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y + y, val ); // 180° to 225°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y + y + 1, val ); // 180° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant5 == 2 )
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Left and bottom clip
        while ( xx > rectOctant5Clipped.x + rectOctant5Clipped.w || yy > rectOctant5Clipped.y + rectOctant5Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx--;
            }
        }
        
        //Bottom and left clip
        while (xx >= limitX && yy >= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y + y, val ); // 180° to 225°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y + y + 1, val ); // 180° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy--;
                x++; xx--;
            }
        }
    }
    
    
    //Octant 6 ------
    x = 0;
    y = iRadius;
    if( drawRectOctant6 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - y, iCenter.y + x, val );  // 270° to 225°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - y - 1, iCenter.y + x, val );  // 270° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant6 == 2 )
    {
        int xx = rectOctant6.x;
        int yy = rectOctant6.y;
        int limitX = rectOctant6Clipped.x + rectOctant6Clipped.w;
        int limitY = rectOctant6Clipped.y + rectOctant6Clipped.h;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Left and bottom clip
        while ( xx < rectOctant6Clipped.x || yy < rectOctant6Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy++;
            }
        }
        
        //Bottom and left clip
        while (xx <= limitX && yy <= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - y, iCenter.y + x, val );  // 270° to 225°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - y - 1, iCenter.y + x, val );  // 270° to 225°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy++;
            }
        }
    }
    
    
    //Octant 7------
    x = 0;
    y = iRadius;
    if( drawRectOctant7 == 1 )
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - y, iCenter.y - x, val ); // 270° to 315°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - y - 1, iCenter.y - x, val ); // 270° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant7 == 2 )
    {
        int xx = rectOctant7.x;
        int yy = rectOctant7.y + rectOctant7.h;
        int limitX = rectOctant7Clipped.x + rectOctant7Clipped.w;
        int limitY = rectOctant7Clipped.y;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Left and bottom clip
        while ( xx < rectOctant7Clipped.x || yy > rectOctant7Clipped.y + rectOctant7Clipped.h )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy--;
            }
        }
        
        //Bottom and left clip
        while (xx <= limitX && yy >= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - y, iCenter.y - x, val ); // 270° to 315°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - y - 1, iCenter.y - x, val ); // 270° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; yy--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; xx++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; xx++;
                x++; yy--;
            }
        }
    }
    
    
    //Octant 8------
    x = 0;
    y = iRadius;
    if( drawRectOctant8 == 1)
    {
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        while (y >= x)
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y - y, val ); // 0° to 315°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y - y - 1, val ); // 0° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
    else if( drawRectOctant8 == 2 )
    {
        int xx = rectOctant8.x + rectOctant8.w;
        int yy = rectOctant8.y;
        int limitX = rectOctant8Clipped.x;
        int limitY = rectOctant8Clipped.y + rectOctant8Clipped.h;
        
        int diff = iRadius - 1;
        int errMax = 2 * (iRadius - 1);
        int errMin = 0;
        
        //Left and bottom clip
        while ( xx > rectOctant8Clipped.x + rectOctant8Clipped.w || yy < rectOctant8Clipped.y )
        {
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx--;
            }
        }
        
        //Bottom and left clip
        while (xx >= limitX && yy <= limitY )
        {
            float alphaTop = FMath::Abs( ( float( diff - errMax ) / float( errMin - errMax ) ) ); //Interpolation of slopedifferential between errMin and errMax
            
            val.SetAlpha8( MaxAlpha * alphaTop );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y - y, val ); // 0° to 315°
            
            val.SetAlpha8( MaxAlpha * (1 - alphaTop ) );
            
            iBlock.SetPixel( iCenter.x - x, iCenter.y - y - 1, val ); // 0° to 315°
            
            if( diff >= ( 2 * x ) )
            {
                diff -= ( 2 * x + 1 );
                x++; xx--;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--; yy++;
            }
            else
            {
                diff += (2 * ( y - x - 1 ) );
                y--; yy++;
                x++; xx--;
            }
        }
    }

    //Filled
    if( iFilled )
    {
        x = 0;
        y = iRadius;
        int diff = iRadius - 1;

        while (y >= x)
        {
            if( diff >= ( 2 * x ) )
            {
                DrawLine( iBlock, FVec2I( iCenter.x + x, iCenter.y - y ), FVec2I( iCenter.x + x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - x, iCenter.y - y ), FVec2I( iCenter.x - x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x + y, iCenter.y - x ), FVec2I( iCenter.x + y, iCenter.y + x ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - y, iCenter.y - x ), FVec2I( iCenter.x - y, iCenter.y + x ), iColor, iClippingRect );
                
                diff -= ( 2 * x + 1 );
                x++;
            }
            else if ( diff < ( 2 * ( iRadius - y ) ) )
            {
                diff += ( 2 * y - 1 );
                y--;
            }
            else
            {
                DrawLine( iBlock, FVec2I( iCenter.x + x, iCenter.y - y ), FVec2I( iCenter.x + x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - x, iCenter.y - y ), FVec2I( iCenter.x - x, iCenter.y + y ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x + y, iCenter.y - x ), FVec2I( iCenter.x + y, iCenter.y + x ), iColor, iClippingRect );
                DrawLine( iBlock, FVec2I( iCenter.x - y, iCenter.y - x ), FVec2I( iCenter.x - y, iCenter.y + x ), iColor, iClippingRect );
                
                diff += (2 * ( y - x - 1 ) );
                y--;
                x++;
            }
        }
    }
}

ULIS_NAMESPACE_END

