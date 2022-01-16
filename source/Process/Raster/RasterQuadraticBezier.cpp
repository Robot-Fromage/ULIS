// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterQuadraticBezier.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementation of non template Quadratic Bezier functions for the raster API
* @license      Please refer to LICENSE.md
*/

#include "Process/Raster/RasterQuadraticBezier.h"

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
    , std::map< int,std::vector< int > >* iStoragePoints
)
{
    FColor val = iColor;

    int sx = x2-x1,sy = y2-y1;
    double dx = x0-x2,dy = y0-y2,xx = x0-x1,yy = y0-y1;
    double xy = xx*sy+yy*sx,cur = xx*sy-yy*sx,err;

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
        } 
        else if(y0 == y2) //Horizontal lines
        {
            for(int i = x0; i < x2; i++)
            {
                (*iStoragePoints)[i - (*iStoragePoints)[0][0]].push_back(y0 - (*iStoragePoints)[0][1]);
            }
        }
        //We don't need to take care of vertical lines, since storagePoints is used to fill an ellipse using the exact same type of vertical lines
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

