// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TextArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Text Command Args.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Color.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTextCommandArgs
class FTextCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FTextCommandArgs() override
    {
    }

    FTextCommandArgs(
          FBlock& iBlock
        , const uint8* iBuffer
        , const FRectI& iRect
        , const std::wstring& iText
        , const FFont& iFont
        , uint32 iFontSize
        , const FColor& iColor
        , const FT_Matrix& iMatrix
        , const FVec2I& iPosition 
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , text( iText )
        , font( iFont )
        , fontSize( iFontSize )
        , color( iColor )
        , matrix( iMatrix )
        , position( iPosition )
    {}

    const std::wstring  text;
    const FFont&        font;
    const uint32        fontSize;
    const FColor        color;
    const FT_Matrix     matrix;
    const FVec2I        position;
};

ULIS_NAMESPACE_END

