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
#include "RasterLine.h"

ULIS_NAMESPACE_BEGIN

void
InternalDrawQuadRationalBezierSeg(
      FBlock& iBlock
    , int x0
    , int y0
    , int x1
    , int y1
    , int x2
    , int y2
    , float w
    , const FColor& iColor
    , const FRectI& iClippingRect
    , std::map< int,std::vector< int > >* iStoragePoints = nullptr
);

void DrawQuadraticBezier(        FBlock&                         iBlock
                               , const FVec2I&                   iCtrlPt0
                               , const FVec2I&                   iCtrlPt1
                               , const FVec2I&                   iCtrlPt2
                               , const float                     iWeight
                               , const FColor&                   iColor
                               , const FRectI&                   iClippingRect );

template< typename T >
void
InternalDrawQuadRationalBezierSegAA(
      FBlock& iBlock
    , int x0
    , int y0
    , int x1
    , int y1
    , int x2
    , int y2
    , float w
    , const FColor& iColor
    , const FRectI& iClippingRect
    , std::map< int, std::vector< int > >* iStoragePoints = nullptr
)
{
    FColor val = iColor;
    T maxAlpha = val.AlphaT<T>();

    int sx = x2-x1,sy = y2-y1;
    double dx = x0-x2,dy = y0-y2,xx = x0-x1,yy = y0-y1;
    double xy = xx*sy+yy*sx,cur = xx*sy-yy*sx,err,ed;
    bool f;

    if(xx*sx > 0.0 || yy*sy > 0.0)
    {
        return;
    }

    if(cur != 0.0 && w > 0.0)
    {
        if(sx*(long)sx+sy*(long)sy > xx*xx+yy*yy)
        {
            x2 = x0; x0 -= int(dx); y2 = y0; y0 -= int(dy); cur = -cur;
        }
        xx = 2.0*(4.0*w*sx*xx+dx*dx);
        yy = 2.0*(4.0*w*sy*yy+dy*dy);
        sx = x0 < x2 ? 1 : -1;
        sy = y0 < y2 ? 1 : -1;
        xy = -2.0*sx*sy*(2.0*w*xy+dx*dy);

        if(cur*sx*sy < 0.0)
        {
            xx = -xx; yy = -yy; xy = -xy; cur = -cur;
        }

        dx = 4.0*w*(x1-x0)*sy*cur+xx/2.0+xy;
        dy = 4.0*w*(y0-y1)*sx*cur+yy/2.0+xy;

        if(w < 0.5 && dy > dx)
        {
            cur = -(w+1.0)/2.0;
            w = sqrt(w);
            xy = 1.0/(w+1.0);
            sx = int(floor((x0+2.0*w*x1+x2)*xy/2.0+0.5));
            sy = int(floor((y0+2.0*w*y1+y2)*xy/2.0+0.5));
            dx = floor((w*x1+x0)*xy+0.5);
            dy = floor((y1*w+y0)*xy+0.5);
            InternalDrawQuadRationalBezierSegAA<T>(iBlock,x0,y0,int(dx),int(dy),sx,sy,float(cur),iColor,iClippingRect,iStoragePoints);
            dx = floor((w*x1+x2)*xy+0.5);
            dy = floor((y1*w+y2)*xy+0.5);
            InternalDrawQuadRationalBezierSegAA<T>(iBlock,sx,sy,int(dx),int(dy),x2,y2,float(cur),iColor,iClippingRect,iStoragePoints);
            return;
        }
        err = dx+dy-xy;

        // Clipping
        FRectI clippingRect = iClippingRect;
        clippingRect.w--;
        clippingRect.h--;

        if(clippingRect.Area() == 0)
        {
            clippingRect = FRectI::FromXYWH(0,0,iBlock.Width() - 1,iBlock.Height() - 1);
        }

        while(dy < dx)
        {
            cur = FMath::Min(dx - xy,xy - dy);
            ed = FMath::Max(dx - xy,xy - dy);
            ed += (2 * ed * cur * cur / (4.0 * ed * ed + cur * cur));

            float errorRatio = float((err - dx - dy + xy) / ed);

            T alphaTop = T(maxAlpha * (1 - FMath::Abs(errorRatio)));
            f = (2 * err + dy) < 0;

            if(alphaTop <= maxAlpha)
            {
                if( x0 >= clippingRect.x && x0 <= (clippingRect.x + clippingRect.w) && y0 >= clippingRect.y && y0 <= (clippingRect.y + clippingRect.h) )
                {
                    val.SetAlphaT<T>(alphaTop);
                    iBlock.SetPixel(x0,y0,val);
                }

                if(iStoragePoints && errorRatio >= 0)
                {
                    (*iStoragePoints)[x0 - (*iStoragePoints)[0][0]].push_back(y0 - (*iStoragePoints)[0][1]);
                }
            }

            if(f)
            {
                if(y0 == y2)
                    return;
                if((dx - err) < ed)
                {
                    float errorRatio = float(1 - (dx - err) / ed);
                    float alpha = FMath::Abs(errorRatio);

                    if(x0 > clippingRect.x && x0 < (clippingRect.x + clippingRect.w) && y0 >= clippingRect.y && y0 <= (clippingRect.y + clippingRect.h))
                    {
                        val.SetAlphaT<T>(T(maxAlpha * alpha));
                        iBlock.SetPixel(x0 + sx,y0,val);
                    }

                    if(iStoragePoints && errorRatio <= 0)
                    {
                        (*iStoragePoints)[x0 + sx - (*iStoragePoints)[0][0]].push_back(y0 - (*iStoragePoints)[0][1]);
                    }
                }
            }

            if(2 * err + dx > 0)
            {
                if(x0 == x2)
                    return;
                if((err - dy) < ed)
                {
                    float errorRatio = float(1 - (err - dy) / ed);
                    float alpha = FMath::Abs(errorRatio);

                    if(x0 >= clippingRect.x && x0 <= (clippingRect.x + clippingRect.w) && y0 > clippingRect.y && y0 < (clippingRect.y + clippingRect.h))
                    {
                        val.SetAlphaT<T>(T(maxAlpha * alpha));
                        iBlock.SetPixel(x0,y0 + sy,val);
                    }

                    if(iStoragePoints && errorRatio >= 0)
                    {
                        (*iStoragePoints)[x0 - (*iStoragePoints)[0][0]].push_back(y0 + sy - (*iStoragePoints)[0][1]);
                    }
                }
                x0 += sx;
                dx += xy;
                err += dy += yy;
            }

            if(f)
            {
                y0 += sy;
                dy += xy;
                err += dx += xx;
            }
        }
    }
    DrawLineAA< T >( iBlock, FVec2I( x0, y0 ), FVec2I( x2, y2 ), iColor, iClippingRect );

    if(iStoragePoints) //We go here only when we draw an ellipse with beziers
    {
        if(x0 == x2 && y0 == y2) //Corners where we draw a single pixel
        {
            (*iStoragePoints)[x0 - (*iStoragePoints)[0][0]].push_back(y0 - (*iStoragePoints)[0][1]);
        } else if(y0 == y2) //Horizontal lines
        {
            for(int i = x0; i < x2; i++)
                (*iStoragePoints)[i - (*iStoragePoints)[0][0]].push_back(y0 - (*iStoragePoints)[0][1]);
        }
        //We don't need to take care of vertical lines, since storagePoints is used to fill an ellipse using the exact same type of vertical lines
    }
}

template< typename T >
void
InternalDrawQuadRationalBezierSegSP(
      FBlock& iBlock
    , double x0
    , double y0
    , double x1
    , double y1
    , double x2
    , double y2
    , double w
    , const FColor& iColor
    , const FRectI& iClippingRect
    , std::map< int, std::vector< int > >* iStoragePoints = nullptr
)
{
    FColor val = iColor;
    T maxAlpha = val.AlphaT<T>();

    double sx = x2 - x1, sy = y2 - y1;
    double dx = x0 - x2, dy = y0 - y2, xx = x0 - x1, yy = y0 - y1;
    double xy = xx * sy + yy * sx, cur = xx * sy - yy * sx, err, ed;
    bool f;

    if (xx*sx > 0.0 || yy * sy > 0.0)
    {
        return;
    }

    if (cur != 0.0 && w > 0.0)
    {
        if (sx*sx + sy*sy > xx*xx + yy*yy)
        {
            x2 = x0; x0 -= dx; y2 = y0; y0 -= dy; cur = -cur;
        }
        xx = 2.0*(4.0*w*sx*xx + dx * dx);
        yy = 2.0*(4.0*w*sy*yy + dy * dy);
        sx = x0 < x2 ? 1 : -1;
        sy = y0 < y2 ? 1 : -1;
        xy = -2.0*sx*sy*(2.0*w*xy + dx * dy);

        if (cur*sx*sy < 0.0)
        {
            xx = -xx; yy = -yy; xy = -xy; cur = -cur;
        }

        dx = 4.0*w*(x1 - x0)*sy*cur + xx / 2.0 + xy;
        dy = 4.0*w*(y0 - y1)*sx*cur + yy / 2.0 + xy;

        if (w < 0.5 && dy > dx)
        {
            cur = -(w + 1.0) / 2.0;
            w = sqrt(w);
            xy = 1.0 / (w + 1.0);
            sx = floor((x0 + 2.0*w*x1 + x2)*xy / 2.0 + 0.5);
            sy = floor((y0 + 2.0*w*y1 + y2)*xy / 2.0 + 0.5);
            dx = floor((w*x1 + x0)*xy /*+ 0.5 ?*/);
            dy = floor((y1*w + y0)*xy /*+ 0.5 ?*/);
            InternalDrawQuadRationalBezierSegSP<T>(iBlock, x0, y0, dx, dy, sx, sy, cur, iColor, iClippingRect, iStoragePoints);
            dx = floor((w*x1 + x2)*xy /*+ 0.5 ?*/);
            dy = floor((y1*w + y2)*xy /*+ 0.5 ?*/);
            InternalDrawQuadRationalBezierSegSP<T>(iBlock, sx, sy, dx, dy, x2, y2, cur, iColor, iClippingRect, iStoragePoints);
            return;
        }
        err = dx + dy - xy;

        // Clipping
        FRectI clippingRect = iClippingRect;
        clippingRect.w--;
        clippingRect.h--;

        if (clippingRect.Area() == 0)
        {
            clippingRect = FRectI::FromXYWH(0, 0, iBlock.Width() - 1, iBlock.Height() - 1);
        }

        while (dy < dx)
        {
            cur = FMath::Min(dx - xy, xy - dy);
            ed = FMath::Max(dx - xy, xy - dy);
            ed += (2 * ed * cur * cur / (4.0 * ed * ed + cur * cur));

            double errorRatio = (err - dx - dy + xy) / ed;

            T alphaTop = T(maxAlpha * (1 - FMath::Abs(errorRatio)));
            f = (2 * err + dy) < 0;

            if (alphaTop <= maxAlpha)
            {
                if (x0 >= clippingRect.x && x0 <= (clippingRect.x + clippingRect.w) && y0 >= clippingRect.y && y0 <= (clippingRect.y + clippingRect.h))
                {
                    val.SetAlphaT<T>(alphaTop);
                    iBlock.SetPixel(int(x0), int(y0), val);
                }

                if (iStoragePoints && errorRatio >= 0)
                {
                    (*iStoragePoints)[int(x0 - (*iStoragePoints)[0][0])].push_back(int(y0 - (*iStoragePoints)[0][1]));
                }
            }

            if (f)
            {
                if (y0 == y2)
                    return;
                if ((dx - err) < ed)
                {
                    float errorRatio = float(1 - (dx - err) / ed);
                    float alpha = FMath::Abs(errorRatio);

                    if (x0 > clippingRect.x && x0 < (clippingRect.x + clippingRect.w) && y0 >= clippingRect.y && y0 <= (clippingRect.y + clippingRect.h))
                    {
                        val.SetAlphaT<T>(T(maxAlpha * alpha));
                        iBlock.SetPixel(int(x0 + sx), int(y0), val);
                    }

                    if (iStoragePoints && errorRatio <= 0)
                    {
                        (*iStoragePoints)[int(x0 + sx - (*iStoragePoints)[0][0])].push_back(int(y0 - (*iStoragePoints)[0][1]));
                    }
                }
            }

            if (2 * err + dx > 0)
            {
                if (x0 == x2)
                    return;
                if ((err - dy) < ed)
                {
                    float errorRatio = float(1 - (err - dy) / ed);
                    float alpha = FMath::Abs(errorRatio);

                    if (x0 >= clippingRect.x && x0 <= (clippingRect.x + clippingRect.w) && y0 > clippingRect.y && y0 < (clippingRect.y + clippingRect.h))
                    {
                        val.SetAlphaT<T>(T(maxAlpha * alpha));
                        iBlock.SetPixel(int(x0), int(y0 + sy), val);
                    }

                    if (iStoragePoints && errorRatio >= 0)
                    {
                        (*iStoragePoints)[int(x0 - (*iStoragePoints)[0][0])].push_back(int(y0 + sy - (*iStoragePoints)[0][1]));
                    }
                }
                x0 += sx;
                dx += xy;
                err += dy += yy;
            }

            if (f)
            {
                y0 += sy;
                dy += xy;
                err += dx += xx;
            }
        }
    }
    DrawLineSP<T>(iBlock, FVec2I(x0, y0), FVec2I(x2, y2), iColor, iClippingRect);

    if (iStoragePoints) //We go here only when we draw an ellipse with beziers
    {
        if (x0 == x2 && y0 == y2) //Corners where we draw a single pixel
        {
            (*iStoragePoints)[int(x0 - (*iStoragePoints)[0][0])].push_back(int(y0 - (*iStoragePoints)[0][1]));
        }
        else if (y0 == y2) //Horizontal lines
        {
            for (int i = int(x0); i < int(x2); i++)
                (*iStoragePoints)[i - (*iStoragePoints)[0][0]].push_back(int(y0 - (*iStoragePoints)[0][1]));
        }
        //We don't need to take care of vertical lines, since storagePoints is used to fill an ellipse using the exact same type of vertical lines
    }
}

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

