// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterRotatedEllipse.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of RotatedEllipse
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include <map>
#include "RasterEllipse.h"
#include "RasterLine.h"
#include "RasterQuadraticBezier.h"
#include "RasterUtils.h"

ULIS_NAMESPACE_BEGIN

void DrawRotatedEllipse(        FBlock&                  iBlock
                              , const FVec2I&            iCenter
                              , const int                iA
                              , const int                iB
                              , const int                iRotationDegrees
                              , const FColor&            iColor
                              , const bool               iFilled
                              , const FRectI&            iClippingRect );

template< typename T >
void DrawRotatedEllipseAA( FBlock&                  iBlock
                                , const FVec2I&            iCenter
                                , const int                iA
                                , const int                iB
                                , const int                iRotationDegrees
                                , const FColor&            iColor
                                , const bool               iFilled
                                , const FRectI&            iClippingRect )
{
    if (iA <= 0 || iB <= 0)
        return;

    if(iRotationDegrees % 180 == 0)
    {
        DrawEllipseAA<T>(iBlock,iCenter,iA,iB,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }
    if(iRotationDegrees % 90 == 0)
    {
        DrawEllipseAA<T>(iBlock,iCenter,iB,iA,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }

    int a = 0;
    int b = 0;
    int rotation = 0;
    int rotationDegreesBase = iRotationDegrees;
    if( iA > iB )
    {
        a = iA;
        b = iB;
        rotation = ((iRotationDegrees % 180) + 180) % 180; //Positive modulo
    }
    else
    {
        rotationDegreesBase+=90;
        a = iB;
        b = iA;
        rotation = ((rotationDegreesBase % 180) + 180 ) % 180; //Positive modulo
    }

    float dx = float(a*a);
    float dy = float(b*b);
    float s = float(std::sin(FMath::DegToRad(rotation)));
    float dz = (dx - dy) * s;
    dx = std::sqrt(dx - dz * s);
    dy = std::sqrt(dy + dz * s);
    a = int(dx + 0.5);
    b = int(dy + 0.5);
    dz = dz * a * b / (dx * dy);

    int x0 = iCenter.x - a;
    int y0 = iCenter.y - b;
    int x1 = iCenter.x + a;
    int y1 = iCenter.y + b;
    dz = float(4 * dz * std::cos(FMath::DegToRad(rotation)));


    dx = float(x1 - x0);
    dy = float(y1 - y0);
    float w = dx * dy;
    if(w != 0.0)
        w = (w - dz) / (w + w);

    if(w > 1 || w < 0)
        return;

    dx = float(std::floor(dx * w + 0.5));
    dy = float(std::floor(dy * w + 0.5));



    if(!iFilled)
    {
        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x0,int(y0 + dy),x0,y0,int(x0 + dx),y0,1 - w,iColor,iClippingRect); //top left
        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x0,int(y0 + dy),x0,y1,int(x1 - dx),y1,w,iColor,iClippingRect); //bottom left
        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x1,int(y1 - dy),x1,y1,int(x1 - dx),y1,1 - w,iColor,iClippingRect); //bottom right
        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x1,int(y1 - dy),x1,y0,int(x0 + dx),y0,w,iColor,iClippingRect); //top right
    } else //Filled Ellipse
    {
        //               x  y
        std::map< int,std::vector< int > > storagePoints; // storagePoints[x][0]  We have n points for each x on the ellipse: p1(x, y0), p2(x, y1) ... pn(x, yn)
                                                            //                 [1]
                                                            //                 [...]
                                                            //                 [n]
        storagePoints[0].push_back(iCenter.x);           // In order to only pass storagePoints in parameter to InternalDrawQuadRationalBezierSeg
        storagePoints[0].push_back(iCenter.y);           // we store the center (useful in this function) at index 0 (we'll know it's there)

        std::map< int,std::vector< int > > pointsForFill; // same structure, but with only the top and bottom points to draw a line on each x to fill the ellipse


        // Lambda ---
        auto fillPointsForFill = [&](bool isTop,int shift)
        {
            //Removing the coordinates of the center we stored
            storagePoints[0].erase(storagePoints[0].begin());
            storagePoints[0].erase(storagePoints[0].begin());

            for(std::map< int,std::vector< int > >::iterator it=storagePoints.begin(); it!=storagePoints.end(); ++it)
            {
                if(it->second.size() > 0)
                {
                    int extremum = it->second[0];
                    for(int i = 1; i < it->second.size() ; i++)
                    {
                        if((isTop && shift == 0) || (shift == 1 && !isTop)) //We get the minimum of it->second
                        {
                            if(extremum > it->second[i])
                                extremum = it->second[i];
                        } else //We get the maximum of it->second
                        {
                            if(extremum < it->second[i])
                                extremum = it->second[i];
                        }
                    }
                    pointsForFill[it->first].push_back(extremum + shift * (isTop ? 1 : -1));
                }
            }

            storagePoints.clear();
            storagePoints[0].push_back(iCenter.x);
            storagePoints[0].push_back(iCenter.y);
        };
        //Lambda end ----


        // Depending of the angle of the ellipse, we either get the outer antialiased pixels or the inner antialiased pixels of the ellipse. If we get the outer pixels, we need to draw from the less outer one on top to the less outer one on bottom, and shift them one pixel inwards so we don't draw on outer pixels at all
        // If we get the inner pixels, we draw from the less inner one on top to the less inner one on bottom, no need to shift the pixels, we're already inside the ellipse, so we can fill from there.

        int shift = ((((rotationDegreesBase + 45) % 180) + 180) % 180) < 90 ? 0 : 1;

        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x0,int(y0 + dy),x0,y0,int(x0 + dx),y0,1 - w,iColor,iClippingRect,&storagePoints); //top left
        fillPointsForFill(true,shift);

        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x0,int(y0 + dy),x0,y1,int(x1 - dx),y1,w,iColor,iClippingRect,&storagePoints); //bottom left
        fillPointsForFill(false,shift);

        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x1,int(y1 - dy),x1,y1,int(x1 - dx),y1,1 - w,iColor,iClippingRect,&storagePoints); //bottom right
        fillPointsForFill(false,shift);

        InternalDrawQuadRationalBezierSegAA<T>(iBlock,x1,int(y1 - dy),x1,y0,int(x0 + dx),y0,w,iColor,iClippingRect,&storagePoints); //top right
        fillPointsForFill(true,shift);

        for(std::map< int,std::vector< int > >::iterator it=pointsForFill.begin(); it!=pointsForFill.end(); ++it)
        {
            if(it->second.size() == 2)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + it->first,iCenter.y + it->second[0]),FVec2I(iCenter.x + it->first,iCenter.y + it->second[1]),iColor,iClippingRect);
            }
            if(it->second.size() > 2) // where we draw more than 2 pixels for a said y (it happens at the junctions between beziers)
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

template< typename T >
void DrawRotatedEllipseSP( FBlock&                  iBlock
                                , const FVec2F&            iCenter
                                , const float              iA
                                , const float              iB
                                , const int                iRotationDegrees
                                , const FColor&            iColor
                                , const bool               iFilled
                                , const FRectI&            iClippingRect )
{
    if (iA <= 0 || iB <= 0)
        return;

    if(iRotationDegrees % 180 == 0)
    {
        DrawEllipseSP<T>(iBlock,iCenter,iA,iB,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }
    if(iRotationDegrees % 90 == 0)
    {
        DrawEllipseSP<T>(iBlock,iCenter,iB,iA,iColor,iFilled,iClippingRect); //Don't bother to use the rotated ellipse algorithm if the ellipse is not rotated
        return;
    }

    float a = 0;
    float b = 0;
    int rotation = 0;
    int rotationDegreesBase = iRotationDegrees;
    if( iA > iB )
    {
        a = iA;
        b = iB;
        rotation = ((iRotationDegrees % 180) + 180) % 180; //Positive modulo
    }
    else
    {
        rotationDegreesBase+=90;
        a = iB;
        b = iA;
        rotation = ((rotationDegreesBase % 180) + 180 ) % 180; //Positive modulo
    }

    float dx = float(a*a);
    float dy = float(b*b);
    float s = float(std::sin(FMath::DegToRad(rotation)));
    float dz = (dx - dy) * s;
    dx = std::sqrt(dx - dz * s);
    dy = std::sqrt(dy + dz * s);
    a = dx + InternalGetPixelBaseAlphaFromCoord( iCenter + FVec2F( iA, 0.5 ) );
    b = dy + InternalGetPixelBaseAlphaFromCoord( iCenter + FVec2F( 0.5, iB ) );
    dz = dz * a * b / (dx * dy);

    float x0 = iCenter.x - a;
    float y0 = iCenter.y - b;
    float x1 = iCenter.x + a;
    float y1 = iCenter.y + b;
    dz = float(4 * dz * std::cos(FMath::DegToRad(rotation)));


    dx = float(x1 - x0);
    dy = float(y1 - y0);
    float w = dx * dy;
    if(w != 0.0)
        w = (w - dz) / (w + w);

    if(w > 1 || w < 0)
        return;

    dx = float(std::floor(dx * w + 0.5));
    dy = float(std::floor(dy * w + 0.5));



    if(!iFilled)
    {
        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x0,int(y0 + dy),x0,y0,int(x0 + dx),y0,1 - w,iColor,iClippingRect); //top left
        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x0,int(y0 + dy),x0,y1,int(x1 - dx),y1,w,iColor,iClippingRect); //bottom left
        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x1,int(y1 - dy),x1,y1,int(x1 - dx),y1,1 - w,iColor,iClippingRect); //bottom right
        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x1,int(y1 - dy),x1,y0,int(x0 + dx),y0,w,iColor,iClippingRect); //top right
    } 
    else //Filled Ellipse
    {
        //               x  y
        std::map< int,std::vector< int > > storagePoints; // storagePoints[x][0]  We have n points for each x on the ellipse: p1(x, y0), p2(x, y1) ... pn(x, yn)
                                                            //                 [1]
                                                            //                 [...]
                                                            //                 [n]
        storagePoints[0].push_back(int(iCenter.x));           // In order to only pass storagePoints in parameter to InternalDrawQuadRationalBezierSeg
        storagePoints[0].push_back(int(iCenter.y));           // we store the center (useful in this function) at index 0 (we'll know it's there)

        std::map< int,std::vector< int > > pointsForFill; // same structure, but with only the top and bottom points to draw a line on each x to fill the ellipse


        // Lambda ---
        auto fillPointsForFill = [&](bool isTop,int shift)
        {
            //Removing the coordinates of the center we stored
            storagePoints[0].erase(storagePoints[0].begin());
            storagePoints[0].erase(storagePoints[0].begin());

            for(std::map< int,std::vector< int > >::iterator it=storagePoints.begin(); it!=storagePoints.end(); ++it)
            {
                if(it->second.size() > 0)
                {
                    int extremum = it->second[0];
                    for(int i = 1; i < it->second.size() ; i++)
                    {
                        if((isTop && shift == 0) || (shift == 1 && !isTop)) //We get the minimum of it->second
                        {
                            if(extremum > it->second[i])
                                extremum = it->second[i];
                        } else //We get the maximum of it->second
                        {
                            if(extremum < it->second[i])
                                extremum = it->second[i];
                        }
                    }
                    pointsForFill[it->first].push_back(extremum + shift * (isTop ? 1 : -1));
                }
            }

            storagePoints.clear();
            storagePoints[0].push_back(int(iCenter.x));
            storagePoints[0].push_back(int(iCenter.y));
        };
        //Lambda end ----


        // Depending of the angle of the ellipse, we either get the outer antialiased pixels or the inner antialiased pixels of the ellipse. If we get the outer pixels, we need to draw from the less outer one on top to the less outer one on bottom, and shift them one pixel inwards so we don't draw on outer pixels at all
        // If we get the inner pixels, we draw from the less inner one on top to the less inner one on bottom, no need to shift the pixels, we're already inside the ellipse, so we can fill from there.

        int shift = ((((rotationDegreesBase + 45) % 180) + 180) % 180) < 90 ? 0 : 1;

        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x0,int(y0 + dy),x0,y0,int(x0 + dx),y0,1 - w,iColor,iClippingRect,&storagePoints); //top left
        fillPointsForFill(true,shift);

        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x0,int(y0 + dy),x0,y1,int(x1 - dx),y1,w,iColor,iClippingRect,&storagePoints); //bottom left
        fillPointsForFill(false,shift);

        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x1,int(y1 - dy),x1,y1,int(x1 - dx),y1,1 - w,iColor,iClippingRect,&storagePoints); //bottom right
        fillPointsForFill(false,shift);

        InternalDrawQuadRationalBezierSegSP<T>(iBlock,x1,int(y1 - dy),x1,y0,int(x0 + dx),y0,w,iColor,iClippingRect,&storagePoints); //top right
        fillPointsForFill(true,shift);

        for(std::map< int,std::vector< int > >::iterator it=pointsForFill.begin(); it!=pointsForFill.end(); ++it)
        {
            if(it->second.size() == 2)
            {
                DrawLine(iBlock,FVec2I(iCenter.x + it->first,iCenter.y + it->second[0]),FVec2I(iCenter.x + it->first,iCenter.y + it->second[1]),iColor,iClippingRect);
            }
            if(it->second.size() > 2) // where we draw more than 2 pixels for a said y (it happens at the junctions between beziers)
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

ULIS_NAMESPACE_END

