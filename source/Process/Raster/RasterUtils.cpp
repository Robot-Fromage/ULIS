// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterUtils.cpp
* @author       Thomas Schmitt
* @brief        This file provides Utility struct and functions for the other raster API files
* @license      Please refer to LICENSE.md
*/
#include "Process/Raster/RasterUtils.h"

ULIS_NAMESPACE_BEGIN

int
InternalComputeCodeForPoint( const FVec2I& iPoint,const FRectI& iClippingRect )
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
bool
InternalCropLineToRectangle( FVec2I& ioP0,FVec2I& ioP1,const FRectI& iClippingRect )
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

float
InternalGetPixelBaseAlphaFromCoord( const FVec2F& iPt )
{
    float xDec = iPt.x - int( iPt.x );
    if( xDec < 0.f )
        xDec+=1.f;

    float xAlpha = 0.f;

    if( xDec <= 0.5f )
        xAlpha = xDec + (0.5f - xDec) / 2;
    else
        xAlpha = 1.f - xDec + (xDec - 0.5f) / 2;

    float yDec = iPt.y - int(iPt.y);
    float yAlpha = 0.f;
    if( yDec < 0.f )
        yDec+=1.f;

    if(yDec <= 0.5f)
        yAlpha = yDec + (0.5f - yDec) / 2;
    else
        yAlpha = 1.f - yDec + (yDec - 0.5f) / 2;

    return ( xAlpha + yAlpha );
}

ULIS_NAMESPACE_END

