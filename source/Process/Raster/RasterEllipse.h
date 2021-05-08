// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterEllipse.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of Ellipse
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include "Process/Raster/RasterUtils.h"
#include "Process/Raster/RasterLine.h"

ULIS_NAMESPACE_BEGIN

void DrawEllipse(         FBlock&                  iBlock
                        , const FVec2I&            iCenter
                        , const int                iA
                        , const int                iB
                        , const FColor&            iColor
                        , const bool               iFilled
                        , const FRectI&            iClippingRect );

template< typename T >
void DrawEllipseAA( FBlock&                  iBlock
                         , const FVec2I&            iCenter
                         , const int                iA
                         , const int                iB
                         , const FColor&            iColor
                         , const bool               iFilled
                         , const FRectI&            iClippingRect )
{
    if (iA <= 0 || iB <= 0)
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

    FRectI rectOctant1 = FRectI(point0.x,point0.y - 1,point45.x - point0.x,point45.y - point0.y);
    FRectI rectOctant2 = FRectI(point45.x + 1,point45.y,point90.x - point45.x,point90.y - point45.y);
    FRectI rectOctant3 = FRectI(point135.x + 1,point90.y,point90.x - point135.x,point135.y - point90.y);
    FRectI rectOctant4 = FRectI(point180.x,point135.y + 1,point135.x - point180.x,point180.y - point135.y);
    FRectI rectOctant5 = FRectI(point225.x,point225.y + 1,point180.x - point225.x,point180.y - point225.y);
    FRectI rectOctant6 = FRectI(point270.x - 1,point270.y,point225.x - point270.x,point225.y - point270.y);
    FRectI rectOctant7 = FRectI(point270.x - 1,point315.y,point315.x - point270.x,point270.y - point315.y);
    FRectI rectOctant8 = FRectI(point315.x,point0.y - 1,point0.x - point315.x,point315.y - point0.y);

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

    T maxAlpha = val.AlphaT<T>();

    int a2 = iA * iA;
    int b2 = iB * iB;
    int fa2 = 4 * a2;
    int fb2 = 4 * b2;
    int x = 0;
    int y = iB;
    int sigma = 2*b2+a2*(-2*iB);

    int errMax = 0;
    int errMin =  2 * (2*b2+a2*(-2*iB));

    //0° is on top and we turn clockwise
    //Octant 1 ------
    if(drawRectOctant1 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));
            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0 to 45 degrees

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y - step,val); // 0 to 45 degrees

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0 to 45 degrees

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y - step,val); // 0 to 45 degrees

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    if(drawRectOctant2 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x + step,iCenter.y - y,val); // 90 to 45 degrees

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x + step,iCenter.y - y,val); // 90 to 45 degrees

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    if(drawRectOctant3 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x + step,iCenter.y + y,val);

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x + step,iCenter.y + y,val);

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
    errMin =  2 * (2*b2+a2*(-2*iB));
    if(drawRectOctant4 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y + step,val); // 180 to 135 degrees

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y + step,val); // 180 to 135 degrees

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
    errMin =  2 * (2*b2+a2*(-2*iB));
    if(drawRectOctant5 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y + step,val);

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y + step,val);

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    if(drawRectOctant6 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x - step,iCenter.y + y,val);

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x - step,iCenter.y + y,val);

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    if(drawRectOctant7 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x - step,iCenter.y - y,val);

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x - step,iCenter.y - y,val);

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
    errMin =  2 * (2*b2+a2*(-2*iB));
    if(drawRectOctant8 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y - step,val);

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
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y - step,val);

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
            int step = sigma <= 0 ? 1 : -1;

            if(step == 1)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);
            } else //step = -1, we draw the aliasing on the inside of the ellipse, so we colorize one pixel less
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y + 1),FVec2I(iCenter.x + x,iCenter.y + y - 1),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y + 1),FVec2I(iCenter.x - x,iCenter.y + y - 1),iColor,iClippingRect);
            }

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
            int step = sigma < 0 ? 1 : -1;

            if(step == 1)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);
            } else //step = -1, we draw the aliasing on the inside of the ellipse, so we colorize one pixel less
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y + 1),FVec2I(iCenter.x + x,iCenter.y + y - 1),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y + 1),FVec2I(iCenter.x - x,iCenter.y + y - 1),iColor,iClippingRect);
            }

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

template< typename T >
void DrawEllipseSP( FBlock&                  iBlock
                         , const FVec2F&            iCenter
                         , const float              iA
                         , const float              iB
                         , const FColor&            iColor
                         , const bool               iFilled
                         , const FRectI&            iClippingRect )
{
    if (iA <= 0 || iB <= 0)
        return;

    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    float angleTo45 = std::atan(- float(iB) / float(iA));
    float shift45x = FMath::Abs(std::cos(angleTo45) * iA);
    float shift45y = FMath::Abs(std::sin(angleTo45) * iB);

    FVec2F point0 =   FVec2F(iCenter.x,iCenter.y - iB);
    FVec2F point45 =  FVec2F(iCenter.x + shift45x,iCenter.y - shift45y);
    FVec2F point90 =  FVec2F(iCenter.x + iA,iCenter.y);
    FVec2F point135 = FVec2F(iCenter.x + shift45x,iCenter.y + shift45y);
    FVec2F point180 = FVec2F(iCenter.x,iCenter.y + iB);
    FVec2F point225 = FVec2F(iCenter.x - shift45x,iCenter.y + shift45y);
    FVec2F point270 = FVec2F(iCenter.x - iA,iCenter.y);
    FVec2F point315 = FVec2F(iCenter.x - shift45x,iCenter.y - shift45y);

    FRectI rectOctant1 = FRectI(int(point0.x),      int(point0.y - 1),  int(point45.x - point0.x),   int(point45.y - point0.y));
    FRectI rectOctant2 = FRectI(int(point45.x + 1), int(point45.y),     int(point90.x - point45.x),  int(point90.y - point45.y));
    FRectI rectOctant3 = FRectI(int(point135.x + 1),int(point90.y),     int(point90.x - point135.x), int(point135.y - point90.y));
    FRectI rectOctant4 = FRectI(int(point180.x),    int(point135.y + 1),int(point135.x - point180.x),int(point180.y - point135.y));
    FRectI rectOctant5 = FRectI(int(point225.x),    int(point225.y + 1),int(point180.x - point225.x),int(point180.y - point225.y));
    FRectI rectOctant6 = FRectI(int(point270.x - 1),int(point270.y),    int(point225.x - point270.x),int(point225.y - point270.y));
    FRectI rectOctant7 = FRectI(int(point270.x - 1),int(point315.y),    int(point315.x - point270.x),int(point270.y - point315.y));
    FRectI rectOctant8 = FRectI(int(point315.x),    int(point0.y - 1),  int(point0.x - point315.x),  int(point315.y - point0.y));

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

    T maxAlpha = val.AlphaT<T>();

    float a2 = iA * iA;
    float b2 = iB * iB;
    float fa2 = 4 * a2;
    float fb2 = 4 * b2;
    float x = 0;
    float y = iB;

    float errMax = 0;
    float errMin =  2 * (2*b2+a2*(-2*iB));
    float sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point0);

    //0° is on top and we turn clockwise
    //Octant 1 ------
    if(drawRectOctant1 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0 to 45 degrees

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x,iCenter.y - y - step,val); // 0 to 45 degrees

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0 to 45 degrees

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x,iCenter.y - y - step,val); // 0 to 45 degrees

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
    errMin = 2 * (2 * a2 + b2 * (1 - 2 * iA));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point90);

    if(drawRectOctant2 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x + step,iCenter.y - y,val); // 90 to 45 degrees

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x + step,iCenter.y - y,val); // 90 to 45 degrees

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point90);

    if(drawRectOctant3 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x + step,iCenter.y + y,val);

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x + step,iCenter.y + y,val);

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
    errMin =  2 * (2*b2+a2*(-2*iB));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point180);

    if(drawRectOctant4 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x,iCenter.y + y + step,val); // 180 to 135 degrees

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x + x,iCenter.y + y + step,val); // 180 to 135 degrees

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
    errMin =  2 * (2*b2+a2*(-2*iB));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point180);

    if(drawRectOctant5 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x,iCenter.y + y + step,val);

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x,iCenter.y + y + step,val);

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point270);

    if(drawRectOctant6 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs((float(sigma - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x - step,iCenter.y + y,val);

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x - step,iCenter.y + y,val);

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
    errMin =  2 * (2*a2+b2*(-2*iA));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point180);

    if(drawRectOctant7 == 1)
    {
        while(a2 * y <= b2 * x)
        {
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x - step,iCenter.y - y,val);

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            int step = sigma < 0 ? 1 : -1;

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x - step,iCenter.y - y,val);

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
    errMin =  2 * (2*b2+a2*(-2*iB));
    sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point0);

    if(drawRectOctant8 == 1)
    {
        while(b2 * x <= a2 * y)
        {
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x,iCenter.y - y - step,val);

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
            float alphaTop = 1 - FMath::Abs(((sigma - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            int step = sigma <= 0 ? 1 : -1;

            iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            iBlock.SetPixelSafe(iCenter.x - x,iCenter.y - y - step,val);

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
        sigma = errMin * InternalGetPixelBaseAlphaFromCoord(point0);

        while(b2 * x <= a2 * y)
        {
            int step = sigma <= 0 ? 1 : -1;

            if(step == 1)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);
            } else //step = -1, we draw the aliasing on the inside of the ellipse, so we colorize one pixel less
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y + 1),FVec2I(iCenter.x + x,iCenter.y + y - 1),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y + 1),FVec2I(iCenter.x - x,iCenter.y + y - 1),iColor,iClippingRect);
            }

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
            int step = sigma < 0 ? 1 : -1;

            if(step == 1)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y),FVec2I(iCenter.x + x,iCenter.y + y),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y),FVec2I(iCenter.x - x,iCenter.y + y),iColor,iClippingRect);
            } else //step = -1, we draw the aliasing on the inside of the ellipse, so we colorize one pixel less
            {
                DrawLine(iBlock,FVec2I(iCenter.x + x,iCenter.y - y + 1),FVec2I(iCenter.x + x,iCenter.y + y - 1),iColor,iClippingRect);
                DrawLine(iBlock,FVec2I(iCenter.x - x,iCenter.y - y + 1),FVec2I(iCenter.x - x,iCenter.y + y - 1),iColor,iClippingRect);
            }

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

