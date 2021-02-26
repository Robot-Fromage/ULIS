// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterAlgorithms.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template functions for the raster API
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#include "RasterAlgorithms.h"

ULIS_NAMESPACE_BEGIN

void DrawCircleAndres(            FBlock&                  iBlock
                                , const FVec2I&            iCenter
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

void DrawCircleBresenham(        FBlock&                  iBlock
                                ,const FVec2I&            iCenter
                                ,const int                iRadius
                                ,const FColor&            iColor
                                ,const bool               iFilled
                                ,const FRectI&            iClippingRect)
{
    //Clipping -----
    int x = 0;
    int y = iRadius;

    FRectI clippingRect = iClippingRect;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    int shift45 = std::sin(FMath::DegToRad(45)) * iRadius;
    FVec2I point0 = FVec2I(iCenter.x,iCenter.y - iRadius);
    FVec2I point45 = FVec2I(iCenter.x + shift45,iCenter.y - shift45);
    FVec2I point90 = FVec2I(iCenter.x + iRadius,iCenter.y);
    FVec2I point135 = FVec2I(iCenter.x + shift45,iCenter.y + shift45);
    FVec2I point180 = FVec2I(iCenter.x,iCenter.y + iRadius);
    FVec2I point225 = FVec2I(iCenter.x - shift45,iCenter.y + shift45);
    FVec2I point270 = FVec2I(iCenter.x - iRadius,iCenter.y);
    FVec2I point315 = FVec2I(iCenter.x - shift45,iCenter.y - shift45);

    FRectI rectOctant1 = FRectI(point0.x,point0.y,point45.x - point0.x,point45.y - point0.y);
    FRectI rectOctant2 = FRectI(point45.x,point45.y,point90.x - point45.x,point90.y - point45.y);
    FRectI rectOctant3 = FRectI(point135.x,point90.y,point90.x - point135.x,point135.y - point90.y);
    FRectI rectOctant4 = FRectI(point180.x,point135.y,point135.x - point180.x,point180.y - point135.y);
    FRectI rectOctant5 = FRectI(point225.x,point225.y,point180.x - point225.x,point180.y - point225.y);
    FRectI rectOctant6 = FRectI(point270.x,point270.y,point225.x - point270.x,point225.y - point270.y);
    FRectI rectOctant7 = FRectI(point270.x,point315.y,point315.x - point270.x,point270.y - point315.y);
    FRectI rectOctant8 = FRectI(point315.x,point0.y,point0.x - point315.x,point315.y - point0.y);

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

    // Drawing ------
    FColor val = iColor;


    //0° is on top and we turn clockwise
    //Octant 1 ------
    if(drawRectOctant1 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant1 == 2)
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;

        int diff = 5 - 4 * iRadius;

        while(xx < rectOctant1Clipped.x || yy < rectOctant1Clipped.y)
        {
            if(diff > 0)
            {
                y--; yy++;
                diff = diff - 8 * y;
            }
            x++; xx++;
            diff = diff + 8 * x + 4;
        }

        while(xx <= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            if(diff > 0)
            {
                y--; yy++;
                diff = diff - 8 * y;
            }
            x++; xx++;
            diff = diff + 8 * x + 4;
        }
    }


    //Octant 2 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant2 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant2 == 2)
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;

        int diff = 5 - 4 * iRadius;
        while(xx > rectOctant2Clipped.x + rectOctant2Clipped.w || yy > rectOctant2Clipped.y + rectOctant2Clipped.h)
        {
            if(diff > 0)
            {
                y--; xx--;
                diff = diff - 8 * y;
            }
            x++; yy--;
            diff = diff + 8 * x + 4;
        }
        while(xx >= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(diff > 0)
            {
                y--; xx--;
                diff = diff - 8 * y;
            }
            x++; yy--;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 3 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant3 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant3 == 2)
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;

        int diff = 5 - 4 * iRadius;
        while(xx > rectOctant3Clipped.x + rectOctant3Clipped.w || yy < rectOctant3Clipped.y)
        {
            if(diff > 0)
            {
                y--; xx--;
                diff = diff - 8 * y;
            }
            x++; yy++;
            diff = diff + 8 * x + 4;
        }
        while(xx >= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(diff > 0)
            {
                y--; xx--;
                diff = diff - 8 * y;
            }
            x++; yy++;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 4 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant4 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant4 == 2)
    {
        int xx = rectOctant4.x;
        int yy = rectOctant4.y + rectOctant4.h;
        int limitX = rectOctant4Clipped.x + rectOctant4Clipped.w;
        int limitY = rectOctant4Clipped.y;

        int diff = 5 - 4 * iRadius;
        while(xx < rectOctant4Clipped.x || yy > rectOctant4Clipped.y + rectOctant4Clipped.h)
        {
            if(diff > 0)
            {
                y--; yy--;
                diff = diff - 8 * y;
            }
            x++; xx++;
            diff = diff + 8 * x + 4;
        }
        while(xx <= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(diff > 0)
            {
                y--; yy--;
                diff = diff - 8 * y;
            }
            x++; xx++;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 5 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant5 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant5 == 2)
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;

        int diff = 5 - 4 * iRadius;
        while(xx > rectOctant5Clipped.x + rectOctant5Clipped.w || yy > rectOctant5Clipped.y + rectOctant5Clipped.h)
        {
            if(diff > 0)
            {
                y--; yy--;
                diff = diff - 8 * y;
            }
            x++; xx--;
            diff = diff + 8 * x + 4;
        }
        while(xx >= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(diff > 0)
            {
                y--; yy--;
                diff = diff - 8 * y;
            }
            x++; xx--;
            diff = diff + 8 * x + 4;
        }
    }


    //Octant 6 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant6 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant6 == 2)
    {
        int xx = rectOctant6.x;
        int yy = rectOctant6.y;
        int limitX = rectOctant6Clipped.x + rectOctant6Clipped.w;
        int limitY = rectOctant6Clipped.y + rectOctant6Clipped.h;

        int diff = 5 - 4 * iRadius;
        while(xx < rectOctant6Clipped.x || yy < rectOctant6Clipped.y)
        {
            if(diff > 0)
            {
                y--; xx++;
                diff = diff - 8 * y;
            }
            x++; yy++;
            diff = diff + 8 * x + 4;
        }
        while(xx <= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(diff > 0)
            {
                y--; xx++;
                diff = diff - 8 * y;
            }
            x++; yy++;
            diff = diff + 8 * x + 4;
        }
    }


    //Octant 7 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant7 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant7 == 2)
    {
        int xx = rectOctant7.x;
        int yy = rectOctant7.y + rectOctant7.h;
        int limitX = rectOctant7Clipped.x + rectOctant7Clipped.w;
        int limitY = rectOctant7Clipped.y;

        int diff = 5 - 4 * iRadius;
        while(xx < rectOctant7Clipped.x || yy > rectOctant7Clipped.y + rectOctant7Clipped.h)
        {
            if(diff > 0)
            {
                y--; xx++;
                diff = diff - 8 * y;
            }
            x++; yy--;
            diff = diff + 8 * x + 4;
        }
        while(xx <= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(diff > 0)
            {
                y--; xx++;
                diff = diff - 8 * y;
            }
            x++; yy--;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 8 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant8 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant8 == 2)
    {
        int xx = rectOctant8.x + rectOctant8.w;
        int yy = rectOctant8.y;
        int limitX = rectOctant8Clipped.x;
        int limitY = rectOctant8Clipped.y + rectOctant8Clipped.h;

        int diff = 5 - 4 * iRadius;
        while(xx > rectOctant8Clipped.x + rectOctant8Clipped.w || yy < rectOctant8Clipped.y)
        {
            if(diff > 0)
            {
                y--; yy++;
                diff = diff - 8 * y;
            }
            x++; xx--;
            diff = diff + 8 * x + 4;
        }
        while(xx >= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(diff > 0)
            {
                y--; yy++;
                diff = diff - 8 * y;
            }
            x++; xx--;
            diff = diff + 8 * x + 4;
        }
    }

    //Fill

    if(iFilled)
    {
        x = 0;
        y = iRadius;
        int diff = 5 - 4 * iRadius;

        while(x <= y)
        {
            DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
            DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);
            DrawLine(iBlock,FVec2I(iCenter.x + y,iCenter.y - x),FVec2I(iCenter.x + y,iCenter.y + x),iColor,iClippingRect);
            DrawLine(iBlock,FVec2I(iCenter.x - y,iCenter.y - x),FVec2I(iCenter.x - y,iCenter.y + x),iColor,iClippingRect);

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    }
}


void DrawArcAndres(           FBlock&                   iBlock
                            , const FVec2I&             iCenter
                            , const int                 iRadius
                            , const int                 iStartDegree
                            , const int                 iEndDegree
                            , const FColor&             iColor
                            , const FRectI&             iClippingRect )
{
    if(iRadius == 0)
        return;

    //Clipping -----
    int x = 0;
    int y = iRadius; //We start from the top of the circle for the first octant

    FRectI clippingRect = iClippingRect;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    int shift45 = std::sin(FMath::DegToRad(45)) * iRadius;
    FVec2I point0 = FVec2I(iCenter.x,iCenter.y - iRadius);
    FVec2I point45 = FVec2I(iCenter.x + shift45,iCenter.y - shift45);
    FVec2I point90 = FVec2I(iCenter.x + iRadius,iCenter.y);
    FVec2I point135 = FVec2I(iCenter.x + shift45,iCenter.y + shift45);
    FVec2I point180 = FVec2I(iCenter.x,iCenter.y + iRadius);
    FVec2I point225 = FVec2I(iCenter.x - shift45,iCenter.y + shift45);
    FVec2I point270 = FVec2I(iCenter.x - iRadius,iCenter.y);
    FVec2I point315 = FVec2I(iCenter.x - shift45,iCenter.y - shift45);

    FRectI rectOctant1 = FRectI(point0.x,point0.y,point45.x - point0.x,point45.y - point0.y);
    FRectI rectOctant2 = FRectI(point45.x,point45.y,point90.x - point45.x,point90.y - point45.y);
    FRectI rectOctant3 = FRectI(point135.x,point90.y,point90.x - point135.x,point135.y - point90.y);
    FRectI rectOctant4 = FRectI(point180.x,point135.y,point135.x - point180.x,point180.y - point135.y);
    FRectI rectOctant5 = FRectI(point225.x,point225.y,point180.x - point225.x,point180.y - point225.y);
    FRectI rectOctant6 = FRectI(point270.x,point270.y,point225.x - point270.x,point225.y - point270.y);
    FRectI rectOctant7 = FRectI(point270.x,point315.y,point315.x - point270.x,point270.y - point315.y);
    FRectI rectOctant8 = FRectI(point315.x,point0.y,point0.x - point315.x,point315.y - point0.y);

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

    int sizeAngleToDraw = (iEndDegree - iStartDegree + 360) % 360; //Positive modulo
    int currentAngle = iStartDegree;

    int octantsToDraw[8] ={0,0,0,0,0,0,0,0}; // 0: Don't draw the octant. 1: draw fully the octant. 2: draw part of the octant
    int directionToDraw[8][2] ={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; // 1 clockwise, -1 anti-clockwise, 2 both 0 irrelevant, second entry is angle to draw on octant

    if(sizeAngleToDraw < 45)
    {
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = 2;
        directionToDraw[currentAngle / 45][1] = sizeAngleToDraw;
    } else if(currentAngle % 45 == 0)
        octantsToDraw[currentAngle / 45] = 1;
    else
    {
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = -1;
        directionToDraw[currentAngle / 45][1] = 45 - (currentAngle % 45);
    }

    sizeAngleToDraw -= (45 - (iStartDegree % 45));

    while(sizeAngleToDraw >= 45)
    {
        currentAngle = (currentAngle + 45) % 360;
        octantsToDraw[currentAngle / 45] = 1;
        sizeAngleToDraw -= 45;
    }

    if(sizeAngleToDraw > 0)
    {
        currentAngle = (currentAngle + 45) % 360;
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = 1;
        directionToDraw[currentAngle / 45][1] = sizeAngleToDraw;
    }

    //0° is on top and we turn clockwise
    //Octant 1 ------
    if(drawRectOctant1 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant1 == 2)
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;

        int diff = iRadius - 1;

        while(xx < rectOctant1Clipped.x || yy < rectOctant1Clipped.y)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy++;
                x++; xx++;
            }
        }

        while(xx <= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy++;
                x++; xx++;
            }
        }
    }

    //Octant 2 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant2 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant2 == 2)
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;

        int diff = iRadius - 1;

        while(xx > rectOctant2Clipped.x + rectOctant2Clipped.w || yy > rectOctant2Clipped.y + rectOctant2Clipped.h)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx--;
                x++; yy--;
            }
        }

        while(xx >= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx--;
                x++; yy--;
            }
        }
    }

    //Octant 3 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant3 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant3 == 2)
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;

        int diff = iRadius - 1;

        //Right and top clip
        while(xx > rectOctant3Clipped.x + rectOctant3Clipped.w || yy < rectOctant3Clipped.y)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx--;
                x++; yy++;
            }
        }

        //Bottom and left clip
        while(xx >= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx--;
                x++; yy++;
            }
        }
    }

    //Octant 4 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant4 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant4 == 2)
    {
        int xx = rectOctant4.x;
        int yy = rectOctant4.y + rectOctant4.h;
        int limitX = rectOctant4Clipped.x + rectOctant4Clipped.w;
        int limitY = rectOctant4Clipped.y;

        int diff = iRadius - 1;

        //Left and bottom clip
        while(xx < rectOctant4Clipped.x || yy > rectOctant4Clipped.y + rectOctant4Clipped.h)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy--;
                x++; xx++;
            }
        }

        //Bottom and left clip
        while(xx <= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy--;
                x++; xx++;
            }
        }
    }


    //Octant 5 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant5 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant5 == 2)
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;

        int diff = iRadius - 1;

        //Left and bottom clip
        while(xx > rectOctant5Clipped.x + rectOctant5Clipped.w || yy > rectOctant5Clipped.y + rectOctant5Clipped.h)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy--;
                x++; xx--;
            }
        }

        //Bottom and left clip
        while(xx >= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy--;
                x++; xx--;
            }
        }
    }


    //Octant 6 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant6 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant6 == 2)
    {
        int xx = rectOctant6.x;
        int yy = rectOctant6.y;
        int limitX = rectOctant6Clipped.x + rectOctant6Clipped.w;
        int limitY = rectOctant6Clipped.y + rectOctant6Clipped.h;

        int diff = iRadius - 1;

        //Left and bottom clip
        while(xx < rectOctant6Clipped.x || yy < rectOctant6Clipped.y)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx++;
                x++; yy++;
            }
        }

        //Bottom and left clip
        while(xx <= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx++;
                x++; yy++;
            }
        }
    }


    //Octant 7 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant7 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant7 == 2)
    {
        int xx = rectOctant7.x;
        int yy = rectOctant7.y + rectOctant7.h;
        int limitX = rectOctant7Clipped.x + rectOctant7Clipped.w;
        int limitY = rectOctant7Clipped.y;

        int diff = iRadius - 1;

        //Left and bottom clip
        while(xx < rectOctant7Clipped.x || yy > rectOctant7Clipped.y + rectOctant7Clipped.h)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx++;
                x++; yy--;
            }
        }

        //Bottom and left clip
        while(xx <= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; yy--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; xx++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; xx++;
                x++; yy--;
            }
        }
    }


    //Octant 8 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant8 == 1)
    {
        int diff = iRadius - 1;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--;
            } else
            {
                diff += (2 * (y - x - 1));
                y--;
                x++;
            }
        }
    } else if(drawRectOctant8 == 2)
    {
        int xx = rectOctant8.x + rectOctant8.w;
        int yy = rectOctant8.y;
        int limitX = rectOctant8Clipped.x;
        int limitY = rectOctant8Clipped.y + rectOctant8Clipped.h;

        int diff = iRadius - 1;

        //Left and bottom clip
        while(xx > rectOctant8Clipped.x + rectOctant8Clipped.w || yy < rectOctant8Clipped.y)
        {
            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy++;
                x++; xx--;
            }
        }

        //Bottom and left clip
        while(xx >= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
            }

            if(diff >= (2 * x))
            {
                diff -= (2 * x + 1);
                x++; xx--;
            } else if(diff < (2 * (iRadius - y)))
            {
                diff += (2 * y - 1);
                y--; yy++;
            } else
            {
                diff += (2 * (y - x - 1));
                y--; yy++;
                x++; xx--;
            }
        }
    }
}


void DrawArcBresenham(        FBlock&                   iBlock
                            , const FVec2I&             iCenter
                            , const int                 iRadius
                            , const int                 iStartDegree
                            , const int                 iEndDegree
                            , const FColor&             iColor
                            , const FRectI&             iClippingRect )
{
    if(iRadius == 0)
        return;

    //Clipping -----
    int x = 0;
    int y = iRadius; //We start from the top of the circle for the first octant

    FRectI clippingRect = iClippingRect;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    int shift45 = std::sin(FMath::DegToRad(45)) * iRadius;
    FVec2I point0 = FVec2I(iCenter.x,iCenter.y - iRadius);
    FVec2I point45 = FVec2I(iCenter.x + shift45,iCenter.y - shift45);
    FVec2I point90 = FVec2I(iCenter.x + iRadius,iCenter.y);
    FVec2I point135 = FVec2I(iCenter.x + shift45,iCenter.y + shift45);
    FVec2I point180 = FVec2I(iCenter.x,iCenter.y + iRadius);
    FVec2I point225 = FVec2I(iCenter.x - shift45,iCenter.y + shift45);
    FVec2I point270 = FVec2I(iCenter.x - iRadius,iCenter.y);
    FVec2I point315 = FVec2I(iCenter.x - shift45,iCenter.y - shift45);

    FRectI rectOctant1 = FRectI(point0.x,point0.y,point45.x - point0.x,point45.y - point0.y);
    FRectI rectOctant2 = FRectI(point45.x,point45.y,point90.x - point45.x,point90.y - point45.y);
    FRectI rectOctant3 = FRectI(point135.x,point90.y,point90.x - point135.x,point135.y - point90.y);
    FRectI rectOctant4 = FRectI(point180.x,point135.y,point135.x - point180.x,point180.y - point135.y);
    FRectI rectOctant5 = FRectI(point225.x,point225.y,point180.x - point225.x,point180.y - point225.y);
    FRectI rectOctant6 = FRectI(point270.x,point270.y,point225.x - point270.x,point225.y - point270.y);
    FRectI rectOctant7 = FRectI(point270.x,point315.y,point315.x - point270.x,point270.y - point315.y);
    FRectI rectOctant8 = FRectI(point315.x,point0.y,point0.x - point315.x,point315.y - point0.y);

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

    int sizeAngleToDraw = (iEndDegree - iStartDegree + 360) % 360; //Positive modulo
    int currentAngle = iStartDegree;

    int octantsToDraw[8] ={0,0,0,0,0,0,0,0}; // 0: Don't draw the octant. 1: draw fully the octant. 2: draw part of the octant
    int directionToDraw[8][2] ={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; // 1 clockwise, -1 anti-clockwise, 2 both 0 irrelevant, second entry is angle to draw on octant

    if(sizeAngleToDraw < 45)
    {
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = 2;
        directionToDraw[currentAngle / 45][1] = sizeAngleToDraw;
    } else if(currentAngle % 45 == 0)
        octantsToDraw[currentAngle / 45] = 1;
    else
    {
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = -1;
        directionToDraw[currentAngle / 45][1] = 45 - (currentAngle % 45);
    }

    sizeAngleToDraw -= (45 - (iStartDegree % 45));

    while(sizeAngleToDraw >= 45)
    {
        currentAngle = (currentAngle + 45) % 360;
        octantsToDraw[currentAngle / 45] = 1;
        sizeAngleToDraw -= 45;
    }

    if(sizeAngleToDraw > 0)
    {
        currentAngle = (currentAngle + 45) % 360;
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = 1;
        directionToDraw[currentAngle / 45][1] = sizeAngleToDraw;
    }

    //0° is on top and we turn clockwise
    //Octant 1 ------
    if(drawRectOctant1 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant1 == 2)
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;

        int diff = 5 - 4 * iRadius;

        while(xx < rectOctant1Clipped.x || yy < rectOctant1Clipped.y)
        {
            if(diff > 0)
            {
                y--; yy++;
                diff = diff - 8 * y;
            }
            x++; xx++;
            diff = diff + 8 * x + 4;
        }

        while(xx <= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
            }

            if(diff > 0)
            {
                y--; yy++;
                diff = diff - 8 * y;
            }
            x++; xx++;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 2 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant2 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant2 == 2)
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;

        int diff = 5 - 4 * iRadius;

        while(xx > rectOctant2Clipped.x + rectOctant2Clipped.w || yy > rectOctant2Clipped.y + rectOctant2Clipped.h)
        {
            if(diff > 0)
            {
                y--; xx--;
                diff = diff - 8 * y;
            }
            x++; yy--;
            diff = diff + 8 * x + 4;
        }

        while(xx >= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
            }

            if(diff > 0)
            {
                y--; xx--;
                diff = diff - 8 * y;
            }
            x++; yy--;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 3 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant3 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant3 == 2)
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;

        int diff = 5 - 4 * iRadius;

        //Right and top clip
        while(xx > rectOctant3Clipped.x + rectOctant3Clipped.w || yy < rectOctant3Clipped.y)
        {
            if(diff > 0)
            {
                y--;xx--;
                diff = diff - 8 * y;
            }
            x++;yy++;
            diff = diff + 8 * x + 4;
        }

        //Bottom and left clip
        while(xx >= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
            }

            if(diff > 0)
            {
                y--;xx--;
                diff = diff - 8 * y;
            }
            x++;yy++;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 4 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant4 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant4 == 2)
    {
        int xx = rectOctant4.x;
        int yy = rectOctant4.y + rectOctant4.h;
        int limitX = rectOctant4Clipped.x + rectOctant4Clipped.w;
        int limitY = rectOctant4Clipped.y;

        int diff = 5 - 4 * iRadius;

        //Left and bottom clip
        while(xx < rectOctant4Clipped.x || yy > rectOctant4Clipped.y + rectOctant4Clipped.h)
        {
            if(diff > 0)
            {
                y--;yy--;
                diff = diff - 8 * y;
            }
            x++;xx++;
            diff = diff + 8 * x + 4;
        }

        //Bottom and left clip
        while(xx <= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
            }

            if(diff > 0)
            {
                y--;yy--;
                diff = diff - 8 * y;
            }
            x++;xx++;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 5 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant5 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant5 == 2)
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;

        int diff = 5 - 4 * iRadius;

        //Left and bottom clip
        while(xx > rectOctant5Clipped.x + rectOctant5Clipped.w || yy > rectOctant5Clipped.y + rectOctant5Clipped.h)
        {
            if(diff > 0)
            {
                y--;yy--;
                diff = diff - 8 * y;
            }
            x++;xx--;
            diff = diff + 8 * x + 4;
        }

        //Bottom and left clip
        while(xx >= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
            }

            if(diff > 0)
            {
                y--;yy--;
                diff = diff - 8 * y;
            }
            x++;xx--;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 6 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant6 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant6 == 2)
    {
        int xx = rectOctant6.x;
        int yy = rectOctant6.y;
        int limitX = rectOctant6Clipped.x + rectOctant6Clipped.w;
        int limitY = rectOctant6Clipped.y + rectOctant6Clipped.h;

        int diff = 5 - 4 * iRadius;

        //Left and bottom clip
        while(xx < rectOctant6Clipped.x || yy < rectOctant6Clipped.y)
        {
            if(diff > 0)
            {
                y--;xx++;
                diff = diff - 8 * y;
            }
            x++;yy++;
            diff = diff + 8 * x + 4;
        }

        //Bottom and left clip
        while(xx <= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
            }

            if(diff > 0)
            {
                y--;xx++;
                diff = diff - 8 * y;
            }
            x++;yy++;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 7 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant7 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant7 == 2)
    {
        int xx = rectOctant7.x;
        int yy = rectOctant7.y + rectOctant7.h;
        int limitX = rectOctant7Clipped.x + rectOctant7Clipped.w;
        int limitY = rectOctant7Clipped.y;

        int diff = 5 - 4 * iRadius;

        //Left and bottom clip
        while(xx < rectOctant7Clipped.x || yy > rectOctant7Clipped.y + rectOctant7Clipped.h)
        {
            if(diff > 0)
            {
                y--;xx++;
                diff = diff - 8 * y;
            }
            x++;yy--;
            diff = diff + 8 * x + 4;
        }

        //Bottom and left clip
        while(xx <= limitX && yy >= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
            }

            if(diff > 0)
            {
                y--;xx++;
                diff = diff - 8 * y;
            }
            x++;yy--;
            diff = diff + 8 * x + 4;
        }
    }

    //Octant 8 ------
    x = 0;
    y = iRadius;
    if(drawRectOctant8 == 1)
    {
        int diff = 5 - 4 * iRadius;

        while(y >= x)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
            }

            if(diff > 0)
            {
                y--;
                diff = diff - 8 * y;
            }
            x++;
            diff = diff + 8 * x + 4;
        }
    } else if(drawRectOctant8 == 2)
    {
        int xx = rectOctant8.x + rectOctant8.w;
        int yy = rectOctant8.y;
        int limitX = rectOctant8Clipped.x;
        int limitY = rectOctant8Clipped.y + rectOctant8Clipped.h;

        int diff = 5 - 4 * iRadius;

        //Left and bottom clip
        while(xx > rectOctant8Clipped.x + rectOctant8Clipped.w || yy < rectOctant8Clipped.y)
        {
            if(diff > 0)
            {
                y--;yy++;
                diff = diff - 8 * y;
            }
            x++;xx--;
            diff = diff + 8 * x + 4;
        }

        //Bottom and left clip
        while(xx >= limitX && yy <= limitY)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
            }

            if(diff > 0)
            {
                y--;yy++;
                diff = diff - 8 * y;
            }
            x++;xx--;
            diff = diff + 8 * x + 4;
        }
    }
}


void DrawEllipse(         FBlock&                  iBlock
                        , const FVec2I&            iCenter
                        , const int                iA
                        , const int                iB
                        , const FColor&            iColor
                        , const bool               iFilled
                        , const FRectI&            iClippingRect )
{
    FRectI clippingRect = iClippingRect;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    float angleTo45 = std::atan(- float(iB) / float(iA));
    int shift45x = FMath::Abs(std::cos(angleTo45) * iA) + 1;
    int shift45y = FMath::Abs(std::sin(angleTo45) * iB) + 1;

    FVec2I point0 = FVec2I(iCenter.x,iCenter.y - iB);
    FVec2I point45 = FVec2I(iCenter.x + shift45x,iCenter.y - shift45y);
    FVec2I point90 = FVec2I(iCenter.x + iA,iCenter.y);
    FVec2I point135 = FVec2I(iCenter.x + shift45x,iCenter.y + shift45y);
    FVec2I point180 = FVec2I(iCenter.x,iCenter.y + iB);
    FVec2I point225 = FVec2I(iCenter.x - shift45x,iCenter.y + shift45y);
    FVec2I point270 = FVec2I(iCenter.x - iA,iCenter.y);
    FVec2I point315 = FVec2I(iCenter.x - shift45x,iCenter.y - shift45y);

    FRectI rectOctant1 = FRectI(point0.x,point0.y,point45.x - point0.x,point45.y - point0.y);
    FRectI rectOctant2 = FRectI(point45.x,point45.y,point90.x - point45.x,point90.y - point45.y);
    FRectI rectOctant3 = FRectI(point135.x,point90.y,point90.x - point135.x,point135.y - point90.y);
    FRectI rectOctant4 = FRectI(point180.x,point135.y,point135.x - point180.x,point180.y - point135.y);
    FRectI rectOctant5 = FRectI(point225.x,point225.y,point180.x - point225.x,point180.y - point225.y);
    FRectI rectOctant6 = FRectI(point270.x,point270.y,point225.x - point270.x,point225.y - point270.y);
    FRectI rectOctant7 = FRectI(point270.x,point315.y,point315.x - point270.x,point270.y - point315.y);
    FRectI rectOctant8 = FRectI(point315.x,point0.y,point0.x - point315.x,point315.y - point0.y);

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

    int a2 = iA * iA;
    int b2 = iB * iB;
    int fa2 = 4 * a2;
    int fb2 = 4 * b2;
    int x = 0;
    int y = iB;
    int sigma = 2*b2+a2*(1-2*iB);

    //0° is on top and we turn clockwise
    //Octant 1 ------
    if(drawRectOctant1 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += b2*(4 * x + 6);
            x++;
        }


    } else if(drawRectOctant1 == 2)
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;

        while(xx < rectOctant1Clipped.x || yy < rectOctant1Clipped.y)
        {
            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy++;
            }
            sigma += b2*(4 * x + 6);
            x++; xx++;
        }

        while(xx <= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy++;
            }
            sigma += b2*(4 * x + 6);
            x++; xx++;
        }
    }

    //Octant 2 ------
    x = iA;
    y = 0;
    sigma = 2*a2+b2*(1-2*iA);
    if(drawRectOctant2 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += a2*(4 * y + 6);
            y++;
        }
    } else if(drawRectOctant2 == 2)
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;

        while(xx > rectOctant2Clipped.x + rectOctant2Clipped.w || yy > rectOctant2Clipped.y + rectOctant2Clipped.h)
        {
            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx--;
            }
            sigma += a2*(4 * y + 6);
            y++; yy--;
        }

        while(xx >= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx--;
            }
            sigma += a2*(4 * y + 6);
            y++; yy--;
        }
    }

    //Octant 3 ------
    x = iA;
    y = 0;
    sigma = 2*a2+b2*(1-2*iA);
    if(drawRectOctant3 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += a2*(4 * y + 6);
            y++;
        }
    } else if(drawRectOctant3 == 2)
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;

        while(xx > rectOctant3Clipped.x + rectOctant3Clipped.w || yy < rectOctant3Clipped.y)
        {
            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx--;
            }
            sigma += a2*(4 * y + 6);
            y++; yy++;
        }

        while(xx >= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx--;
            }
            sigma += a2*(4 * y + 6);
            y++; yy++;
        }
    }

    //Octant 4 ------
    x = 0;
    y = iB;
    sigma = 2*b2+a2*(1-2*iB);
    if(drawRectOctant4 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += b2*(4 * x + 6);
            x++;
        }
    } else if(drawRectOctant4 == 2)
    {
        int xx = rectOctant4.x;
        int yy = rectOctant4.y + rectOctant4.h;
        int limitX = rectOctant4Clipped.x + rectOctant4Clipped.w;
        int limitY = rectOctant4Clipped.y;

        while(xx < rectOctant4Clipped.x || yy > rectOctant4Clipped.y + rectOctant4Clipped.h)
        {
            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy--;
            }
            sigma += b2*(4 * x + 6);
            x++; xx++;
        }

        while(xx <= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy--;
            }
            sigma += b2*(4 * x + 6);
            x++; xx++;
        }
    }

    //Octant 5 ------
    x = 0;
    y = iB;
    sigma = 2*b2+a2*(1-2*iB);
    if(drawRectOctant5 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += b2*(4 * x + 6);
            x++;
        }
    } else if(drawRectOctant5 == 2)
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;

        while(xx > rectOctant5Clipped.x + rectOctant5Clipped.w || yy > rectOctant5Clipped.y + rectOctant5Clipped.h)
        {
            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy--;
            }
            sigma += b2*(4 * x + 6);
            x++; xx--;
        }

        while(xx >= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy--;
            }
            sigma += b2*(4 * x + 6);
            x++; xx--;
        }
    }

    //Octant 6 ------
    x = iA;
    y = 0;
    sigma = 2*a2+b2*(1-2*iA);
    if(drawRectOctant6 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += a2*(4 * y + 6);
            y++;
        }
    } else if(drawRectOctant6 == 2)
    {
        int xx = rectOctant6.x;
        int yy = rectOctant6.y;
        int limitX = rectOctant6Clipped.x + rectOctant6Clipped.w;
        int limitY = rectOctant6Clipped.y + rectOctant6Clipped.h;

        while(xx < rectOctant6Clipped.x || yy < rectOctant6Clipped.y)
        {
            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx++;
            }
            sigma += a2*(4 * y + 6);
            y++; yy++;
        }

        while(xx <= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx++;
            }
            sigma += a2*(4 * y + 6);
            y++; yy++;
        }
    }

    //Octant 7 ------
    x = iA;
    y = 0;
    sigma = 2*a2+b2*(1-2*iA);
    if(drawRectOctant7 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += a2*(4 * y + 6);
            y++;
        }
    } else if(drawRectOctant7 == 2)
    {
        int xx = rectOctant7.x;
        int yy = rectOctant7.y + rectOctant7.h;
        int limitX = rectOctant7Clipped.x + rectOctant7Clipped.w;
        int limitY = rectOctant7Clipped.y;

        while(xx < rectOctant7Clipped.x || yy > rectOctant7Clipped.y + rectOctant7Clipped.h)
        {
            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx++;
            }
            sigma += a2*(4 * y + 6);
            y++; yy--;
        }

        while(xx <= limitX && yy >= limitY)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--; xx++;
            }
            sigma += a2*(4 * y + 6);
            y++; yy--;
        }
    }


    //Octant 8 ------
    x = 0;
    y = iB;
    sigma = 2*b2+a2*(1-2*iB);
    if(drawRectOctant8 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += b2*(4 * x + 6);
            x++;
        }
    } else if(drawRectOctant8 == 2)
    {
        int xx = rectOctant8.x + rectOctant8.w;
        int yy = rectOctant8.y;
        int limitX = rectOctant8Clipped.x;
        int limitY = rectOctant8Clipped.y + rectOctant8Clipped.h;

        while(xx > rectOctant8Clipped.x + rectOctant8Clipped.w || yy < rectOctant8Clipped.y)
        {
            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy++;
            }
            sigma += b2*(4 * x + 6);
            x++; xx--;
        }

        while(xx >= limitX && yy <= limitY)
        {
            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--; yy++;
            }
            sigma += b2*(4 * x + 6);
            x++; xx--;
        }
    }

    //Fill

    if(iFilled)
    {
        x = 0;
        y = iB;
        sigma = 2*b2+a2*(1-2*iB);

        while(b2 * x <= a2 * y)
        {
            DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
            DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);

            if(sigma >= 0)
            {
                sigma += fa2 * (1 - y);
                y--;
            }
            sigma += b2*(4 * x + 6);
            x++;
        }

        x = iA;
        y = 0;
        sigma = 2*a2+b2*(1-2*iA);

        while(a2 * y <= b2 * x)
        {
            DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
            DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);

            if(sigma >= 0)
            {
                sigma += fb2 * (1 - x);
                x--;
            }
            sigma += a2*(4 * y + 6);
            y++;
        }
    }
}


void DrawRotatedEllipse(        FBlock&                  iBlock
                              , const FVec2I&            iCenter
                              , const int                iA
                              , const int                iB
                              , const int                iRotationDegrees
                              , const FColor&            iColor
                              , const bool               iFilled
                              , const FRectI&            iClippingRect )
{
    if(iRotationDegrees % 180 == 0)
    {
        DrawEllipse(iBlock,iCenter,iA,iB,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }
    if(iRotationDegrees % 90 == 0)
    {
        DrawEllipse(iBlock,iCenter,iB,iA,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }

    //               x  y
    std::map< int,std::vector< int > > storagePoints; // storagePoints[x][0]  We have two points for each x on the ellipse: p1(x, y0), p2(x, y1)
                                                        //                 [1]
    storagePoints[0].push_back(iCenter.x);           // In order to only pass storagePoints in parameter to InternalDrawQuadRationalBezierSeg
    storagePoints[0].push_back(iCenter.y);           // we store the center (useful in this function) at index 0 (we'll know it's there)


    int a = iA;
    int b = iB;

    float dx = (long)iA*iA;
    float dy = (long)iB*iB;
    float s = std::sin(FMath::DegToRad(iRotationDegrees));
    float dz = (dx - dy) * s;
    dx = std::sqrt(dx - dz * s);
    dy = std::sqrt(dy + dz * s);
    a = dx + 0.5;
    b = dy + 0.5;
    dz = dz * a * b / (dx * dy);

    int x0 = iCenter.x - a;
    int y0 = iCenter.y - b;
    int x1 = iCenter.x + a;
    int y1 = iCenter.y + b;
    dz = (4 * dz * std::cos(FMath::DegToRad(iRotationDegrees)));


    dx = x1 - x0;
    dy = y1 - y0;
    float w = dx * dy;
    if(w != 0.0)
        w = (w - dz) / (w + w);

    if(w > 1 || w < 0)
        return;

    dx = std::floor(dx * w + 0.5);
    dy = std::floor(dy * w + 0.5);

    InternalDrawQuadRationalBezierSeg(iBlock,x0,y0 + dy,x0,y0,x0 + dx,y0,1 - w,iColor,iClippingRect,&storagePoints);
    InternalDrawQuadRationalBezierSeg(iBlock,x0,y0 + dy,x0,y1,x1 - dx,y1,w,iColor,iClippingRect,&storagePoints);
    InternalDrawQuadRationalBezierSeg(iBlock,x1,y1 - dy,x1,y1,x1 - dx,y1,1 - w,iColor,iClippingRect,&storagePoints);
    InternalDrawQuadRationalBezierSeg(iBlock,x1,y1 - dy,x1,y0,x0 + dx,y0,w,iColor,iClippingRect,&storagePoints);

    if(iFilled) //We fill the ellipse by drawing vertical lines
    {
        //We delete the values we stored for the center position (two times pop_front)
        storagePoints[0].erase(storagePoints[0].begin());
        storagePoints[0].erase(storagePoints[0].begin());

        for(std::map< int,std::vector< int > >::iterator it=storagePoints.begin(); it!=storagePoints.end(); ++it)
        {
            if(it->second.size() == 2)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + it->first,iCenter.y + it->second[0]),FVec2I(iCenter.x + it->first,iCenter.y + it->second[1]),iColor,iClippingRect);
            }
            if(it->second.size() > 2) // where we draw more than 2 pixels for a said y
            {
                int minY = it->second[0];
                int maxY = it->second[0];
                for(int i = 1; i < it->second.size(); i++)
                {
                    if(minY > it->second[i])
                        minY = it->second[i];

                    if(maxY < it->second[i])
                        maxY = it->second[i];
                }
                DrawLine(iBlock,FVec2I(iCenter.x + it->first,iCenter.y + minY),FVec2I(iCenter.x + it->first,iCenter.y + maxY),iColor,iClippingRect);
            }
        }
    }
}


void DrawRectangle(        FBlock&                        iBlock
                         , const FVec2I&                  iTopLeft
                         , const FVec2I&                  iBottomRight
                         , const FColor&                  iColor
                         //, const bool                     iFilled // Done in invocation
                         , const FRectI&                  iClippingRect )
{
    FRectI clippingRect = iClippingRect;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    const int xmin = FMath::Min3(iTopLeft.x,iBottomRight.x,clippingRect.x + clippingRect.w);
    const int ymin = FMath::Min3(iTopLeft.y,iBottomRight.y,clippingRect.y + clippingRect.h);
    const int xmax = FMath::Max3(iTopLeft.x,iBottomRight.x,clippingRect.x);
    const int ymax = FMath::Max3(iTopLeft.y,iBottomRight.y,clippingRect.y);
    const int width = FMath::Max(xmax - xmin,0);
    const int height = FMath::Max(ymax - ymin,0);
    /*if(iFilled)
    {
        const FRectI rect = FRectI(xmin,ymin,width,height);

    } else
    {*/
        DrawLine(iBlock,FVec2I(xmin,ymin),FVec2I(xmax,ymin),iColor,iClippingRect); // Top
        DrawLine(iBlock,FVec2I(xmax,ymin),FVec2I(xmax,ymax),iColor,iClippingRect); // Right
        DrawLine(iBlock,FVec2I(xmin,ymax),FVec2I(xmax,ymax),iColor,iClippingRect); // Bot
        DrawLine(iBlock,FVec2I(xmin,ymin),FVec2I(xmin,ymax),iColor,iClippingRect); // Left
    //}
}


void DrawPolygon(        FBlock&                      iBlock
                       , const std::vector< FVec2I >& iPoints
                       , const FColor&                iColor
                       , const bool                   iFilled
                       , const FRectI&                iClippingRect )
{
    if(iPoints.size() < 3)
        return;

    int j = iPoints.size() - 1;
    for(int i = 0; i < iPoints.size(); i++)
    {
        DrawLine(iBlock,iPoints.at(i),iPoints.at(j),iColor,iClippingRect);
        j = i;
    }


    if(iFilled)
    {
        int maxX = 0;
        int maxY = 0;
        int minX = INT_MAX;
        int minY = INT_MAX;

        //Initialization of useful variables
        for(int i = 0; i < iPoints.size(); i++)
        {
            if(maxX < iPoints[i].x)
                maxX = iPoints[i].x;
            if(maxY < iPoints[i].y)
                maxY = iPoints[i].y;
            if(minX > iPoints[i].x)
                minX = iPoints[i].x;
            if(minY > iPoints[i].y)
                minY = iPoints[i].y;
        }

        //We go through the polygon by scanning it top to bottom
        for(int y = minY; y <= maxY; y++)
        {
            std::vector< int > nodesX;
            int j = iPoints.size() - 1;

            for(int i = 0; i < iPoints.size(); i++)
            {
                if((iPoints[i].y < y && iPoints[j].y >= y) || (iPoints[j].y < y && iPoints[i].y >= y))
                {
                    nodesX.push_back(iPoints[i].x  + double(y - iPoints[i].y) / double(iPoints[j].y - iPoints[i].y) * (iPoints[j].x - iPoints[i].x));
                }
                j = i;
            }

            //Sorting the nodes on X
            int i = 0;
            int size = nodesX.size() - 1;
            while(i < size)
            {
                if(nodesX[i] > nodesX[i+1])
                {
                    int temp = nodesX[i];
                    nodesX[i]=nodesX[i+1];
                    nodesX[i+1] = temp;
                    if(i > 0)
                        i--;
                } else
                {
                    i++;
                }
            }

            //Filling the polygon on line y
            for(i = 0; i < nodesX.size(); i+= 2)
            {
                if(nodesX[i] > maxX) break;
                if(nodesX[i+1] > minX)
                {
                    if(nodesX[i] < minX)
                        nodesX[i] = minX;
                    if(nodesX[i+1] > maxX)
                        nodesX[i+1] = maxX;

                    DrawLine(iBlock,FVec2I(nodesX[i],y),FVec2I(nodesX[i+1],y),iColor,iClippingRect);
                }
            }
        }
    }
}


void DrawQuadraticBezier(        FBlock&                         iBlock
                               , const FVec2I&                   iCtrlPt0
                               , const FVec2I&                   iCtrlPt1
                               , const FVec2I&                   iCtrlPt2
                               , const float                     iWeight
                               , const FColor&                   iColor
                               , const FRectI&                   iClippingRect )
{
    int x = iCtrlPt0.x - 2 * iCtrlPt1.x + iCtrlPt2.x;
    int y = iCtrlPt0.y - 2 * iCtrlPt1.y + iCtrlPt2.y;
    double dx = iCtrlPt0.x - iCtrlPt1.x;
    double dy = iCtrlPt0.y - iCtrlPt1.y;
    double dWeight;
    double dt;
    double dq;

    FVec2I pt0 = iCtrlPt0;
    FVec2I pt1 = iCtrlPt1;
    FVec2I pt2 = iCtrlPt2;

    float weight = iWeight;

    if(weight < 0) //Can't draw a bezier curve with a weight < 0
        return;

    if(dx * (pt2.x - pt1.x) > 0)
    {
        if(dy * (pt2.y - pt1.y) > 0)
        {
            if(FMath::Abs(dx * y) > FMath::Abs(dy * x))
            {
                pt0.x = pt2.x;
                pt2.x = dx + pt1.x;
                pt0.y = pt2.y;
                pt2.y = dy + pt1.y;
            }
        }
        if(pt0.x == pt2.x || weight == 1.0)
        {
            dt = (pt0.x - pt1.x) / (double)x;
        } else
        {
            dq = std::sqrt(4.0 * weight * weight * (pt0.x - pt1.x) * (pt2.x - pt1.x) + (pt2.x - pt0.x) * (long)(pt2.x - pt0.x));

            if(pt1.x < pt0.x)
                dq = -dq;

            dt = (2.0 * weight * (pt0.x - pt1.x) - pt0.x + pt2.x + dq) / (2.0 * (1.0 - weight) * (pt2.x - pt0.x));
        }
        dq = 1.0 / (2.0 * dt * (1.0 - dt) * (weight - 1.0) + 1.0);
        dx = (dt * dt * (pt0.x - 2.0 * weight * pt1.x + pt2.x) + 2.0 * dt * (weight * pt1.x - pt0.x) + pt0.x) * dq;
        dy = (dt * dt * (pt0.y - 2.0 * weight * pt1.y + pt2.y) + 2.0 * dt * (weight * pt1.y - pt0.y) + pt0.y) * dq;
        dWeight = dt * (weight - 1.0) + 1.0;
        dWeight *= (dWeight * dq);
        weight = ((1.0 - dt) * (weight - 1.0) + 1.0) * std::sqrt(dq);
        x = std::floor(dx + 0.5);
        y = std::floor(dy + 0.5);
        dy = (dx - pt0.x) * (pt1.y - pt0.y) / (pt1.x - pt0.x) + pt0.y;
        InternalDrawQuadRationalBezierSeg(iBlock,pt0.x,pt0.y,x,std::floor(dy + 0.5),x,y,dWeight,iColor,iClippingRect);
        dy = (dx - pt2.x) * (pt1.y - pt2.y) / (pt1.x - pt2.x) + pt2.y;
        pt1.y = std::floor(dy + 0.5);
        pt0.x = pt1.x = x;
        pt0.y = y;

    }

    if((pt0.y - pt1.y) * (long)(pt2.y - pt1.y) > 0)
    {
        if(pt0.y == pt2.y || iWeight == 1.0)
        {
            dt = (pt0.y - pt1.y) / (pt0.y - 2.0 * pt1.y + pt2.y);
        } else
        {
            dq = std::sqrt(4.0 * weight * weight * (pt0.y - pt1.y) * (pt2.y - pt1.y) + (pt2.y - pt0.y) * (long)(pt2.y - pt0.y));

            if(pt1.y < pt0.y)
                dq = -dq;

            dt = (2.0 * weight * (pt0.y - pt1.y) - pt0.y + pt2.y + dq) / (2.0 * (1.0 - weight) * (pt2.y - pt0.y));
        }
        dq = 1.0 / (2.0 * dt * (1.0 - dt) * (weight - 1.0) + 1.0);
        dx = (dt * dt * (pt0.x - 2.0 * weight * pt1.x + pt2.x) + 2.0 * dt * (weight * pt1.x - pt0.x) + pt0.x) * dq;
        dy = (dt * dt * (pt0.y - 2.0 * weight * pt1.y + pt2.y) + 2.0 * dt * (weight * pt1.y - pt0.y) + pt0.y) * dq;
        dWeight = dt * (weight - 1.0) + 1.0;
        dWeight *= (dWeight * dq);
        weight = ((1.0 - dt) * (weight - 1.0) + 1.0) * std::sqrt(dq);
        x = std::floor(dx + 0.5);
        y = std::floor(dy + 0.5);
        dx = (pt1.x - pt0.x) * (dy - pt0.y) / (pt1.y - pt0.y) + pt0.x;
        InternalDrawQuadRationalBezierSeg(iBlock,pt0.x,pt0.y,std::floor(dx + 0.5),y,x,y,dWeight,iColor,iClippingRect);

        dx = (pt1.x - pt2.x) * (dy - pt2.y) / (pt1.y - pt2.y) + pt2.x;
        pt1.x = std::floor(dx + 0.5);
        pt0.x = x;
        pt0.y = pt1.y = y;

    }
    InternalDrawQuadRationalBezierSeg(iBlock,pt0.x,pt0.y,pt1.x,pt1.y,pt2.x,pt2.y,weight * weight,iColor,iClippingRect);
}


ULIS_NAMESPACE_END

