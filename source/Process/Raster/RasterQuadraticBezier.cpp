// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterQuadraticBezier.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Quadratic Bezier functions for the raster API
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/

#include "RasterQuadraticBezier.h"

ULIS_NAMESPACE_BEGIN

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
                pt2.x = int(dx) + pt1.x;
                pt0.y = pt2.y;
                pt2.y = int(dy) + pt1.y;
            }
        }
        if(pt0.x == pt2.x || weight == 1.0f)
        {
            dt = (pt0.x - pt1.x) / (double)x;
        } else
        {
            dq = std::sqrt(4.0f * weight * weight * (pt0.x - pt1.x) * (pt2.x - pt1.x) + (pt2.x - pt0.x) * (long)(pt2.x - pt0.x));

            if(pt1.x < pt0.x)
                dq = -dq;

            dt = (2.0f * weight * (pt0.x - pt1.x) - pt0.x + pt2.x + dq) / (2.0f * (1.0f - weight) * (pt2.x - pt0.x));
        }
        dq = 1.0f / (2.0f * dt * (1.0f - dt) * (weight - 1.0f) + 1.0f);
        dx = (dt * dt * (pt0.x - 2.0f * weight * pt1.x + pt2.x) + 2.0f * dt * (weight * pt1.x - pt0.x) + pt0.x) * dq;
        dy = (dt * dt * (pt0.y - 2.0f * weight * pt1.y + pt2.y) + 2.0f * dt * (weight * pt1.y - pt0.y) + pt0.y) * dq;
        dWeight = dt * (weight - 1.0f) + 1.0f;
        dWeight *= (dWeight * dq);
        weight = float(((1.0f - dt) * (weight - 1.0f) + 1.0f) * std::sqrt(dq));
        x = int(std::floor(dx + 0.5f));
        y = int(std::floor(dy + 0.5f));
        dy = (dx - pt0.x) * (pt1.y - pt0.y) / (pt1.x - pt0.x) + pt0.y;
        InternalDrawQuadRationalBezierSeg(iBlock,pt0.x,pt0.y,x,int(std::floor(dy + 0.5f)),x,y,float(dWeight),iColor,iClippingRect);
        dy = (dx - pt2.x) * (pt1.y - pt2.y) / (pt1.x - pt2.x) + pt2.y;
        pt1.y = int(std::floor(dy + 0.5f));
        pt0.x = pt1.x = x;
        pt0.y = y;

    }

    if((pt0.y - pt1.y) * (long)(pt2.y - pt1.y) > 0)
    {
        if(pt0.y == pt2.y || iWeight == 1.0f)
        {
            dt = (pt0.y - pt1.y) / (pt0.y - 2.0f * pt1.y + pt2.y);
        } else
        {
            dq = std::sqrt(4.0f * weight * weight * (pt0.y - pt1.y) * (pt2.y - pt1.y) + (pt2.y - pt0.y) * (long)(pt2.y - pt0.y));

            if(pt1.y < pt0.y)
                dq = -dq;

            dt = (2.0f * weight * (pt0.y - pt1.y) - pt0.y + pt2.y + dq) / (2.0f * (1.0f - weight) * (pt2.y - pt0.y));
        }
        dq = 1.0f / (2.0f * dt * (1.0f - dt) * (weight - 1.0f) + 1.0f);
        dx = (dt * dt * (pt0.x - 2.0f * weight * pt1.x + pt2.x) + 2.0f * dt * (weight * pt1.x - pt0.x) + pt0.x) * dq;
        dy = (dt * dt * (pt0.y - 2.0f * weight * pt1.y + pt2.y) + 2.0f * dt * (weight * pt1.y - pt0.y) + pt0.y) * dq;
        dWeight = dt * (weight - 1.0f) + 1.0f;
        dWeight *= (dWeight * dq);
        weight = float(((1.0f - dt) * (weight - 1.0f) + 1.0f) * std::sqrt(dq));
        x = int(std::floor(dx + 0.5f));
        y = int(std::floor(dy + 0.5f));
        dx = (pt1.x - pt0.x) * (dy - pt0.y) / (pt1.y - pt0.y) + pt0.x;
        InternalDrawQuadRationalBezierSeg(iBlock,pt0.x,pt0.y,int(std::floor(dx + 0.5f)),y,x,y,float(dWeight),iColor,iClippingRect);

        dx = (pt1.x - pt2.x) * (dy - pt2.y) / (pt1.y - pt2.y) + pt2.x;
        pt1.x = int(std::floor(dx + 0.5f));
        pt0.x = x;
        pt0.y = pt1.y = y;

    }
    InternalDrawQuadRationalBezierSeg(iBlock,pt0.x,pt0.y,pt1.x,pt1.y,pt2.x,pt2.y,weight * weight,iColor,iClippingRect);
}

ULIS_NAMESPACE_END

