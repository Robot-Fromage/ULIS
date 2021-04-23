// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterEllipse.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Ellipse functions for the raster API
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#include "Process/Raster/RasterEllipse.h"
#include "Process/Raster/RasterLine.h"

ULIS_NAMESPACE_BEGIN

void DrawEllipse(         
      FBlock& iBlock
    , const FVec2I& iCenter
    , const int iA
    , const int iB
    , const FColor& iColor
    , const bool iFilled
    , const FRectI& iClippingRect
)
{
    if( iA <= 0 || iB <= 0 )
        return;

    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    float angleTo45 = std::atan(- float(iB) / float(iA));
    int shift45x = int(FMath::Abs(std::cos(angleTo45) * iA));
    int shift45y = int(FMath::Abs(std::sin(angleTo45) * iB));

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
    int sigma = 2*b2+a2*(-2*iB);

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
    } 
    else if(drawRectOctant1 == 2)
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
    sigma = 2*a2+b2*(-2*iA);
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
    } 
    else if(drawRectOctant2 == 2)
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
    sigma = 2*a2+b2*(-2*iA);
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
    } 
    else if(drawRectOctant3 == 2)
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
    sigma = 2*b2+a2*(-2*iB);
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
    } 
    else if(drawRectOctant4 == 2)
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
    sigma = 2*b2+a2*(-2*iB);
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
    }
    else if(drawRectOctant5 == 2)
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
    sigma = 2*a2+b2*(-2*iA);
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
    } 
    else if(drawRectOctant6 == 2)
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
    sigma = 2*a2+b2*(-2*iA);
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
    }
    else if(drawRectOctant7 == 2)
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
    sigma = 2*b2+a2*(-2*iB);
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
    } 
    else if(drawRectOctant8 == 2)
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
        sigma = 2*b2+a2*(-2*iB);

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
        sigma = 2*a2+b2*(-2*iA);

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

ULIS_NAMESPACE_END

