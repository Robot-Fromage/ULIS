// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterArcAndres.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of the Arc Andres
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include "RasterUtils.h"

ULIS_NAMESPACE_BEGIN

void DrawArcAndres(           FBlock&                   iBlock
                            , const FVec2I&             iCenter
                            , const int                 iRadius
                            , const int                 iStartDegree
                            , const int                 iEndDegree
                            , const FColor&             iColor
                            , const FRectI&             iClippingRect );

template< typename T >
static void DrawArcAndresAA(  FBlock&                   iBlock
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
    int errMax = 2 * (iRadius - 1);
    int errMin = 0;

    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    int shift45 = int(std::sin(FMath::DegToRad(45)) * iRadius);
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

    T maxAlpha = val.AlphaT<T>();

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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y - y - 1,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y - x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y - x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y,iCenter.y + x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + y + 1,iCenter.y + x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x + x,iCenter.y + y + 1,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y + y + 1,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y + x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y + x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y,iCenter.y - x,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - y - 1,iCenter.y - x,val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y - 1,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
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
            float alphaTop = FMath::Abs((float(diff - errMax) / float(errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y,val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y - 1,val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(iCenter.x - x,iCenter.y - y - 1,val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(iCenter.x - x,iCenter.y - y - 1,val);
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

template< typename T >
static void DrawArcAndresSP(  FBlock&                   iBlock
                            , const FVec2F&             iCenter
                            , const float               iRadius
                            , const int                 iStartDegree
                            , const int                 iEndDegree
                            , const FColor&             iColor
                            , const FRectI&             iClippingRect )
{
    if(iRadius == 0)
        return;

    //Clipping -----
    float x = 0;
    float y = iRadius; //We start from the top of the circle for the first octant
    float errMax = 2 * (iRadius - 1);
    float errMin = 0;

    FRectI clippingRect = iClippingRect;
    clippingRect.w--;
    clippingRect.h--;

    if(clippingRect.Area() == 0)
    {
        clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
    }

    int shift45 = int(std::sin(FMath::DegToRad(45)) * iRadius);
    FVec2F point0 =   FVec2F(iCenter.x, iCenter.y - iRadius);
    FVec2F point45 =  FVec2F(iCenter.x + shift45,iCenter.y - shift45);
    FVec2F point90 =  FVec2F(iCenter.x + iRadius,iCenter.y);
    FVec2F point135 = FVec2F(iCenter.x + shift45,iCenter.y + shift45);
    FVec2F point180 = FVec2F(iCenter.x,iCenter.y + iRadius);
    FVec2F point225 = FVec2F(iCenter.x - shift45,iCenter.y + shift45);
    FVec2F point270 = FVec2F(iCenter.x - iRadius,iCenter.y);
    FVec2F point315 = FVec2F(iCenter.x - shift45,iCenter.y - shift45);

    FRectI rectOctant1 = FRectI(int(point0.x),   int(point0.y),   int(point45.x - point0.x),    int(point45.y - point0.y));
    FRectI rectOctant2 = FRectI(int(point45.x),  int(point45.y),  int(point90.x - point45.x),   int(point90.y - point45.y));
    FRectI rectOctant3 = FRectI(int(point135.x), int(point90.y),  int(point90.x - point135.x),  int(point135.y - point90.y));
    FRectI rectOctant4 = FRectI(int(point180.x), int(point135.y), int(point135.x - point180.x), int(point180.y - point135.y));
    FRectI rectOctant5 = FRectI(int(point225.x), int(point225.y), int(point180.x - point225.x), int(point180.y - point225.y));
    FRectI rectOctant6 = FRectI(int(point270.x), int(point270.y), int(point225.x - point270.x), int(point225.y - point270.y));
    FRectI rectOctant7 = FRectI(int(point270.x), int(point315.y), int(point315.x - point270.x), int(point270.y - point315.y));
    FRectI rectOctant8 = FRectI(int(point315.x), int(point0.y),   int(point0.x - point315.x),   int(point315.y - point0.y));

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

    int sizeAngleToDraw = (iEndDegree - iStartDegree + 360) % 360; //Positive modulo
    int currentAngle = iStartDegree;

    int octantsToDraw[8] ={0,0,0,0,0,0,0,0}; // 0: Don't draw the octant. 1: draw fully the octant. 2: draw part of the octant
    int directionToDraw[8][2] ={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; // 1 clockwise, -1 anti-clockwise, 2 both, 0 irrelevant, second entry is angle to draw on octant

    if(sizeAngleToDraw < 45)
    {
        octantsToDraw[currentAngle / 45] = 2;
        directionToDraw[currentAngle / 45][0] = 2;
        directionToDraw[currentAngle / 45][1] = sizeAngleToDraw;
    } 
    else if(currentAngle % 45 == 0)
    {
        octantsToDraw[currentAngle / 45] = 1;
    }
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point0 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(int(round(iCenter.x + x)), int(round(iCenter.y - y)), val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)), int(round(iCenter.y - y)), val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)), int(round(iCenter.y - y)), val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)), int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val);
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
    } 
    else if(drawRectOctant1 == 2)
    {
        int xx = rectOctant1.x;
        int yy = rectOctant1.y;
        int limitX = rectOctant1Clipped.w + rectOctant1Clipped.x;
        int limitY = rectOctant1Clipped.h + rectOctant1Clipped.y;

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point0 );

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y)),val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y)),val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y)),val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[0] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val); // 0° to 45°

            // Complex cases
            if(octantsToDraw[0] == 2)
            {
                if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y - y - 1)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point90 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val);
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
    } 
    else if(drawRectOctant2 == 2)
    {
        int xx = rectOctant2.x + rectOctant2.w;
        int yy = rectOctant2.y + rectOctant2.h;
        int limitX = rectOctant2Clipped.x;
        int limitY = rectOctant2Clipped.y;

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point90 );

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y - x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[1] == 1) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val); // 90° to 45°

            if(octantsToDraw[1] == 2)
            {
                if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y - x)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point90 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val);
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
    } 
    else if(drawRectOctant3 == 2)
    {
        int xx = rectOctant3.x + rectOctant3.w;
        int yy = rectOctant3.y;
        int limitX = rectOctant3Clipped.x;
        int limitY = rectOctant3Clipped.y + rectOctant3Clipped.h;

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord(point90);

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y)),int(round(iCenter.y + x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[2] == 1) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val); // 90° to 135°

            if(octantsToDraw[2] == 2)
            {
                if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + y + 1)),int(round(iCenter.y + x)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point180 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val);
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

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord(point180);

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[3] == 1) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val); // 180° to 135°

            if(octantsToDraw[3] == 2)
            {
                if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x + x)),int(round(iCenter.y + y + 1)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point180 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val);
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
    } 
    else if(drawRectOctant5 == 2)
    {
        int xx = rectOctant5.x + rectOctant5.w;
        int yy = rectOctant5.y + rectOctant5.h;
        int limitX = rectOctant5Clipped.x;
        int limitY = rectOctant5Clipped.y;

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord(point180);

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[4] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val); // 180° to 225°

            if(octantsToDraw[4] == 2)
            {
                if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val);
                else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y + y + 1)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point270 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);
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

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point270 );

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y + x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[5] == 1) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);  // 270° to 225°

            if(octantsToDraw[5] == 2)
            {
                if(directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);
                else if(directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y + x)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point270 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val);
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

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point270 );

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y)),int(round(iCenter.y - x)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[6] == 1) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val); // 270° to 315°

            if(octantsToDraw[6] == 2)
            {
                if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val);
                else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - y - 1)),int(round(iCenter.y - x)),val);
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
        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point0 );

        while(y >= x)
        {
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
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

        float diff = errMax * InternalGetPixelBaseAlphaFromCoord( point0 );

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
            float alphaTop = FMath::Abs(((diff - errMax) / (errMin - errMax))); //Interpolation of slopedifferential between errMin and errMax

            val.SetAlphaT<T>(T(maxAlpha * alphaTop));

            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y)),val);
            }

            val.SetAlphaT<T>(T(maxAlpha * (1 - alphaTop)));

            if(octantsToDraw[7] == 1) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val); // 0° to 315°

            if(octantsToDraw[7] == 2)
            {
                if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
                else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) iBlock.SetPixel(int(round(iCenter.x - x)),int(round(iCenter.y - y - 1)),val);
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

ULIS_NAMESPACE_END

