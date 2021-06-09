﻿// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Arc.h
* @author       Thomas Schmitt
* @brief        This file provides the Arc points generation methods
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN


static inline void GenerateArcPoints(
      const FVec2I& iCenter
    , const int iRadius
    , const int iStartDegree
    , const int iEndDegree
    , TArray<FVec2I>& ioArcPoints )
{
    if(iRadius == 0)
        return;

    int x = 0;
    int y = iRadius; //We start from the top of the circle for the first octant
    int octantStartingIndex = 0;

    int sizeAngleToDraw = (iEndDegree - iStartDegree + 360) % 360; //Positive modulo
    int currentAngle = iStartDegree;

    int octantsToDraw[8] ={0,0,0,0,0,0,0,0}; // 0: Don't draw the octant. 1: draw fully the octant. 2: draw part of the octant
    int directionToDraw[8][2] ={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; // 1 clockwise, -1 anti-clockwise, 2 both from inside, 3 both from outside, 0 irrelevant, second entry is angle to draw on octant

    if(sizeAngleToDraw < 45 && iStartDegree / 45 == iEndDegree / 45)
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
        directionToDraw[currentAngle / 45][1] = sizeAngleToDraw;

        if (directionToDraw[currentAngle / 45][0] == 0)
            directionToDraw[currentAngle / 45][0] = 1;
        else
            directionToDraw[currentAngle / 45][0] = 3;
    }

    //0� is on top and we turn clockwise
    //Octant 1 ------
    int diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[0] == 1) 
        {
            ioArcPoints.PushBack( FVec2I( iCenter.x + x, iCenter.y - y ));
        }
        else if(octantsToDraw[0] == 2) //complex case
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[0][0] == 1 && currentAngleOnFirstOctant < directionToDraw[0][1]) ioArcPoints.PushBack( FVec2I( iCenter.x + x, iCenter.y - y ));
            else if(directionToDraw[0][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[0][1]) ioArcPoints.PushBack( FVec2I( iCenter.x + x, iCenter.y - y ));
            else if(directionToDraw[0][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I( iCenter.x + x, iCenter.y - y ));
            else if (directionToDraw[0][0] == 3 && currentAngleOnFirstOctant < (iStartDegree % 45) != currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I( iCenter.x + x, iCenter.y - y ));
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }

    octantStartingIndex = ioArcPoints.Size();
    
    //Octant 2 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[1] == 1) 
        {
            ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + y, iCenter.y - x ));
        }
        else if(octantsToDraw[1] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[1][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[1][1]) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + y, iCenter.y - x ));
            else if(directionToDraw[1][0] == -1 && currentAngleOnFirstOctant < directionToDraw[1][1]) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + y, iCenter.y - x ));
            else if(directionToDraw[1][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + y, iCenter.y - x ));
            else if (directionToDraw[1][0] == 3 && 45 - currentAngleOnFirstOctant < (iStartDegree % 45) != 45 - currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + y, iCenter.y - x ));
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }

    octantStartingIndex = ioArcPoints.Size();

    //Octant 3 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[2] == 1) 
        {
            ioArcPoints.PushBack( FVec2I(iCenter.x + y, iCenter.y + x) );
        }
        else if(octantsToDraw[2] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[2][0] == 1 && currentAngleOnFirstOctant < directionToDraw[2][1]) ioArcPoints.PushBack( FVec2I(iCenter.x + y, iCenter.y + x) );
            else if(directionToDraw[2][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[2][1]) ioArcPoints.PushBack( FVec2I(iCenter.x + y, iCenter.y + x) );
            else if(directionToDraw[2][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I(iCenter.x + y, iCenter.y + x) );
            else if (directionToDraw[2][0] == 3 && currentAngleOnFirstOctant < (iStartDegree % 45) != currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I(iCenter.x + y, iCenter.y + x) );
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        } 
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }

    octantStartingIndex = ioArcPoints.Size();

    //Octant 4 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[3] == 1)
        {
            ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + x, iCenter.y + y ));
        }
        else if(octantsToDraw[3] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[3][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[3][1]) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + x, iCenter.y + y ));
            else if(directionToDraw[3][0] == -1 && currentAngleOnFirstOctant < directionToDraw[3][1]) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + x, iCenter.y + y ));
            else if(directionToDraw[3][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + x, iCenter.y + y ));
            else if (directionToDraw[3][0] == 3 && 45 - currentAngleOnFirstOctant < (iStartDegree % 45) != 45 - currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x + x, iCenter.y + y ));
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }

    octantStartingIndex = ioArcPoints.Size();

    //Octant 5 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[4] == 1)
        {
            ioArcPoints.PushBack( FVec2I( iCenter.x - x, iCenter.y + y ));
        }
        else if(octantsToDraw[4] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[4][0] == 1 && currentAngleOnFirstOctant < directionToDraw[4][1]) ioArcPoints.PushBack( FVec2I( iCenter.x - x, iCenter.y + y ));
            else if(directionToDraw[4][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[4][1]) ioArcPoints.PushBack( FVec2I( iCenter.x - x, iCenter.y + y ));
            else if(directionToDraw[4][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I( iCenter.x - x, iCenter.y + y ));
            else if (directionToDraw[4][0] == 3 && currentAngleOnFirstOctant < (iStartDegree % 45) != currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I( iCenter.x - x, iCenter.y + y ));
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }

    octantStartingIndex = ioArcPoints.Size();

    //Octant 6 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while (y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if (octantsToDraw[5] == 1)
        {
            ioArcPoints.Insert(octantStartingIndex, FVec2I(iCenter.x - y, iCenter.y + x));
        }
        else if (octantsToDraw[5] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if (directionToDraw[5][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[5][1]) ioArcPoints.Insert(octantStartingIndex, FVec2I(iCenter.x - y, iCenter.y + x));
            else if (directionToDraw[5][0] == -1 && currentAngleOnFirstOctant < directionToDraw[5][1]) ioArcPoints.Insert(octantStartingIndex, FVec2I(iCenter.x - y, iCenter.y + x));
            else if (directionToDraw[5][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.Insert(octantStartingIndex, FVec2I(iCenter.x - y, iCenter.y + x));
            else if (directionToDraw[5][0] == 3 && 45 - currentAngleOnFirstOctant < (iStartDegree % 45) != 45 - currentAngleOnFirstOctant >(iEndDegree % 45)) ioArcPoints.Insert(octantStartingIndex, FVec2I(iCenter.x - y, iCenter.y + x));
        }

        if (diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }
        
    octantStartingIndex = ioArcPoints.Size();

    //Octant 7 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[6] == 1) 
        {
            ioArcPoints.PushBack( FVec2I( iCenter.x - y, iCenter.y - x ));
        }
        else if(octantsToDraw[6] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[6][0] == 1 && currentAngleOnFirstOctant < directionToDraw[6][1]) ioArcPoints.PushBack( FVec2I( iCenter.x - y, iCenter.y - x ));
            else if(directionToDraw[6][0] == -1 && 45 - currentAngleOnFirstOctant < directionToDraw[6][1]) ioArcPoints.PushBack( FVec2I( iCenter.x - y, iCenter.y - x ));
            else if(directionToDraw[6][0] == 2 && currentAngleOnFirstOctant > (iStartDegree % 45) && currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I( iCenter.x - y, iCenter.y - x ));
            else if (directionToDraw[6][0] == 3 && currentAngleOnFirstOctant < (iStartDegree % 45) != currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.PushBack( FVec2I( iCenter.x - y, iCenter.y - x ));
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }

    octantStartingIndex = ioArcPoints.Size();

    //Octant 8 ------
    x = 0;
    y = iRadius;
    diff = iRadius;

    while(y >= x)
    {
        if (diff < (2 * (iRadius - y)))
        {
            diff += (2 * y - 1);
            y--;
        }

        if(octantsToDraw[7] == 1) 
        {
            ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x - x, iCenter.y - y ));
        }
        else if(octantsToDraw[7] == 2)
        {
            double currentAngleOnFirstOctant = -::ULIS::FMath::RadToDeg(std::acos(double(x) / double(iRadius)) - (FMath::kPId / 2));

            if(directionToDraw[7][0] == 1 && 45 - currentAngleOnFirstOctant < directionToDraw[7][1]) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x - x, iCenter.y - y ));
            else if(directionToDraw[7][0] == -1 && currentAngleOnFirstOctant < directionToDraw[7][1]) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x - x, iCenter.y - y ));
            else if(directionToDraw[7][0] == 2 && 45 - currentAngleOnFirstOctant > (iStartDegree % 45) && 45 - currentAngleOnFirstOctant < (iEndDegree % 45)) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x - x, iCenter.y - y ));
            else if (directionToDraw[7][0] == 3 && 45 - currentAngleOnFirstOctant < (iStartDegree % 45) != 45 - currentAngleOnFirstOctant > (iEndDegree % 45)) ioArcPoints.Insert( octantStartingIndex, FVec2I( iCenter.x - x, iCenter.y - y ));
        }

        if(diff >= (2 * x))
        {
            diff -= (2 * x + 1);
            x++;
        }
        else
        {
            diff += (2 * (y - x - 1));
            y--;
            x++;
        }
    }
}

ULIS_NAMESPACE_END
