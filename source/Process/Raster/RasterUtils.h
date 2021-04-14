// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterUtils.h
* @author       Thomas Schmitt
* @brief        This file provides Utility struct and functions for the other raster API files
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Image/Block.h"
#include <map>
#include "RasterLine.h"

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

static const float InternalGetPixelBaseAlphaFromCoord( const FVec2F& iPt )
{
    float xDec = iPt.x - int( iPt.x );
    float xAlpha = 0.f;

    if( xDec <= 0.5f )
        xAlpha = xDec + (0.5f - xDec) / 2;
    else
        xAlpha = 1.f - xDec + (xDec - 0.5f) / 2;

    float yDec = iPt.y - int(iPt.y);
    float yAlpha = 0.f;

    if(yDec <= 0.5f)
        yAlpha = yDec + (0.5f - yDec) / 2;
    else
        yAlpha = 1.f - yDec + (yDec - 0.5f) / 2;

    return ( xAlpha + yAlpha );
}

static void InternalDrawQuadRationalBezierSeg( FBlock& iBlock
                                             , int x0
                                             , int y0
                                             , int x1
                                             , int y1
                                             , int x2
                                             , int y2
                                             , float w
                                             , const FColor& iColor
                                             , const FRectI& iClippingRect
                                             , std::map< int,std::vector< int > >* iStoragePoints = NULL)
{
    FColor val = iColor;

    int sx = x2-x1,sy = y2-y1;
    double dx = x0-x2,dy = y0-y2,xx = x0-x1,yy = y0-y1;
    double xy = xx*sy+yy*sx,cur = xx*sy-yy*sx,err;

    if(xx*sx > 0.0 || yy*sy > 0.0)
    {
        return;
    }

    if(cur != 0.0 && w > 0.0) {
        if(sx*(long)sx+sy*(long)sy > xx*xx+yy*yy) {
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
            InternalDrawQuadRationalBezierSeg(iBlock,x0,y0,int(dx),int(dy),sx,sy,float(cur),iColor,iClippingRect,iStoragePoints);
            dx = floor((w*x1+x2)*xy+0.5);
            dy = floor((y1*w+y2)*xy+0.5);
            InternalDrawQuadRationalBezierSeg(iBlock,sx,sy,int(dx),int(dy),x2,y2,float(cur),iColor,iClippingRect,iStoragePoints);
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

        FRectI bezierRect = FRectI::FromMinMax(FMath::Min3(x0,x1,x2),FMath::Min3(y0,y1,y2),FMath::Max3(x0,x1,x2),FMath::Max3(y0,y1,y2));

        clippingRect = clippingRect & bezierRect;

        while(dy <= xy && dx >= xy)
        {
            if( x0 >= clippingRect.x && x0 <= (clippingRect.x + clippingRect.w) && y0 >= clippingRect.y && y0 <= (clippingRect.y + clippingRect.h) )
                iBlock.SetPixel(x0,y0,val);

            if(iStoragePoints)
            {
                (*iStoragePoints)[x0 - (*iStoragePoints)[0][0]].push_back(y0 - (*iStoragePoints)[0][1]);
            }
            if(x0 == x2 && y0 == y2)
                return;
            x1 = 2 * err > dy; y1 = 2*(err + yy) < -dy;
            if(2 * err < dx || y1) { y0 += sy; dy += xy; err += dx += xx; }
            if(2 * err > dx || x1) { x0 += sx; dx += xy; err += dy += yy; }
        }
    }
    DrawLine(iBlock,FVec2I(x0,y0),FVec2I(x2,y2),iColor,iClippingRect);

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
static void InternalDrawQuadRationalBezierSegAA(FBlock& iBlock
                                              ,int x0
                                              ,int y0
                                              ,int x1
                                              ,int y1
                                              ,int x2
                                              ,int y2
                                              ,float w
                                              ,const FColor& iColor
                                              ,const FRectI& iClippingRect
                                              ,std::map< int,std::vector< int > >* iStoragePoints = NULL)
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
    DrawLineAA<T>(iBlock,FVec2I(x0,y0),FVec2I(x2,y2),iColor,iClippingRect);

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
static void InternalDrawQuadRationalBezierSegSP(FBlock& iBlock
                                              , double x0
                                              , double y0
                                              , double x1
                                              , double y1
                                              , double x2
                                              , double y2
                                              , double w
                                              , const FColor& iColor
                                              , const FRectI& iClippingRect
                                              , std::map< int, std::vector< int > >* iStoragePoints = NULL)
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

ULIS_NAMESPACE_END

