// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterQuadraticBezier.h
* @author       Thomas Schmitt
* @brief        This file provides the actual functions for the raster of Quadratic Bezier
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include "RasterUtils.h"

ULIS_NAMESPACE_BEGIN

void DrawQuadraticBezier(        FBlock&                         iBlock
                               , const FVec2I&                   iCtrlPt0
                               , const FVec2I&                   iCtrlPt1
                               , const FVec2I&                   iCtrlPt2
                               , const float                     iWeight
                               , const FColor&                   iColor
                               , const FRectI&                   iClippingRect );

template< typename T >
static void DrawQuadraticBezierAA( FBlock&                         iBlock
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
                pt2.x = int(dx + pt1.x);
                pt0.y = pt2.y;
                pt2.y = int(dy + pt1.y);
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
        weight = float(((1.0 - dt) * (weight - 1.0) + 1.0) * std::sqrt(dq));
        x = int(std::floor(dx + 0.5));
        y = int(std::floor(dy + 0.5));
        dy = (dx - pt0.x) * (pt1.y - pt0.y) / (pt1.x - pt0.x) + pt0.y;
        InternalDrawQuadRationalBezierSegAA<T>(iBlock,pt0.x,pt0.y,x,int(std::floor(dy + 0.5)),x,y,float(dWeight),iColor,iClippingRect);
        dy = (dx - pt2.x) * (pt1.y - pt2.y) / (pt1.x - pt2.x) + pt2.y;
        pt1.y = int(std::floor(dy + 0.5));
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
        weight = float(((1.0 - dt) * (weight - 1.0) + 1.0) * std::sqrt(dq));
        x = int(std::floor(dx + 0.5));
        y = int(std::floor(dy + 0.5));
        dx = (pt1.x - pt0.x) * (dy - pt0.y) / (pt1.y - pt0.y) + pt0.x;
        InternalDrawQuadRationalBezierSegAA<T>(iBlock,pt0.x,pt0.y,int(std::floor(dx + 0.5)),y,x,y,float(dWeight),iColor,iClippingRect);

        dx = (pt1.x - pt2.x) * (dy - pt2.y) / (pt1.y - pt2.y) + pt2.x;
        pt1.x = int(std::floor(dx + 0.5));
        pt0.x = x;
        pt0.y = pt1.y = y;
    }
    InternalDrawQuadRationalBezierSegAA<T>(iBlock,pt0.x,pt0.y,pt1.x,pt1.y,pt2.x,pt2.y,weight * weight,iColor,iClippingRect);
}

template< typename T >
static void DrawQuadraticBezierSP( FBlock&                         iBlock
                                 , const FVec2F&                   iCtrlPt0
                                 , const FVec2F&                   iCtrlPt1
                                 , const FVec2F&                   iCtrlPt2
                                 , const float                     iWeight
                                 , const FColor&                   iColor
                                 , const FRectI&                   iClippingRect )
{
    double x = iCtrlPt0.x - 2 * iCtrlPt1.x + iCtrlPt2.x;
    double y = iCtrlPt0.y - 2 * iCtrlPt1.y + iCtrlPt2.y;
    double dx = iCtrlPt0.x - iCtrlPt1.x;
    double dy = iCtrlPt0.y - iCtrlPt1.y;
    double dWeight;
    double dt;
    double dq;

    FVec2F pt0 = iCtrlPt0;
    FVec2F pt1 = iCtrlPt1;
    FVec2F pt2 = iCtrlPt2;

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
                pt2.x = float(dx + pt1.x);
                pt0.y = pt2.y;
                pt2.y = float(dy + pt1.y);
            }
        }
        if(pt0.x == pt2.x || weight == 1.0)
        {
            dt = (pt0.x - pt1.x) / (double)x;
        } 
        else
        {
            dq = std::sqrt(4.0 * weight * weight * (pt0.x - pt1.x) * (pt2.x - pt1.x) + (pt2.x - pt0.x) * (pt2.x - pt0.x));

            if(pt1.x < pt0.x)
                dq = -dq;

            dt = (2.0 * weight * (pt0.x - pt1.x) - pt0.x + pt2.x + dq) / (2.0 * (1.0 - weight) * (pt2.x - pt0.x));
        }
        dq = 1.0 / (2.0 * dt * (1.0 - dt) * (weight - 1.0) + 1.0);
        dx = (dt * dt * (pt0.x - 2.0 * weight * pt1.x + pt2.x) + 2.0 * dt * (weight * pt1.x - pt0.x) + pt0.x) * dq;
        dy = (dt * dt * (pt0.y - 2.0 * weight * pt1.y + pt2.y) + 2.0 * dt * (weight * pt1.y - pt0.y) + pt0.y) * dq;
        dWeight = dt * (weight - 1.0) + 1.0;
        dWeight *= (dWeight * dq);
        weight = float(((1.0 - dt) * (weight - 1.0) + 1.0) * std::sqrt(dq));
        x = dx + InternalGetPixelBaseAlphaFromCoord( FVec2F(dx, 0.5) );
        y = dy + InternalGetPixelBaseAlphaFromCoord( FVec2F(0.5, dy) );
        dy = (dx - pt0.x) * (pt1.y - pt0.y) / (pt1.x - pt0.x) + pt0.y;
        InternalDrawQuadRationalBezierSegSP<T>(iBlock,pt0.x,pt0.y,x,dy + InternalGetPixelBaseAlphaFromCoord( FVec2F(0.5, dy) ),x,y,dWeight,iColor,iClippingRect);
        dy = (dx - pt2.x) * (pt1.y - pt2.y) / (pt1.x - pt2.x) + pt2.y;
        pt1.y = float(dy + InternalGetPixelBaseAlphaFromCoord( FVec2F(0.5, dy) ));
        pt0.x = pt1.x = float(x);
        pt0.y = float(y);
    }

    if((pt0.y - pt1.y) * (long)(pt2.y - pt1.y) > 0)
    {
        if(pt0.y == pt2.y || iWeight == 1.0)
        {
            dt = (pt0.y - pt1.y) / (pt0.y - 2.0 * pt1.y + pt2.y);
        } 
        else
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
        weight = float(((1.0 - dt) * (weight - 1.0) + 1.0) * std::sqrt(dq));
        x = dx + InternalGetPixelBaseAlphaFromCoord( FVec2F(dx, 0.5) );
        y = dy + InternalGetPixelBaseAlphaFromCoord( FVec2F(0.5, dy) );
        dx = (pt1.x - pt0.x) * (dy - pt0.y) / (pt1.y - pt0.y) + pt0.x;
        InternalDrawQuadRationalBezierSegSP<T>(iBlock,pt0.x,pt0.y,dx + InternalGetPixelBaseAlphaFromCoord( FVec2F(dx, 0.5) ),y,x,y,float(dWeight),iColor,iClippingRect);

        dx = (pt1.x - pt2.x) * (dy - pt2.y) / (pt1.y - pt2.y) + pt2.x;
        pt1.x = float(dx + InternalGetPixelBaseAlphaFromCoord( FVec2F(dx, 0.5) ));
        pt0.x = float(x);
        pt0.y = pt1.y = float(y);
    }
    InternalDrawQuadRationalBezierSegSP<T>(iBlock,pt0.x,pt0.y,pt1.x,pt1.y,pt2.x,pt2.y,weight * weight,iColor,iClippingRect);
}

ULIS_NAMESPACE_END

