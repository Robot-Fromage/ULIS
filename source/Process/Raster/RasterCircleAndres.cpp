// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterCircleAndres.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Circle Andres functions for the raster API
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Raster/RasterCircleAndres.h"

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
    clippingRect.w--;
    clippingRect.h--;
    
    if( clippingRect.Area() == 0 )
    {
        clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
    }
    
    int shift45 = int(std::sin( FMath::DegToRadF( 45.f ) ) * iRadius);
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

ULIS_NAMESPACE_END

