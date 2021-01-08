// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Branding.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the Branding entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Misc/Branding.h"
#include "Core/AXI.h"
#include "Image/Block.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
void ApplyBranding( FBlock& iBlock, const FString& iStr ) {
    const char* str = iStr.Data();
    uint64 len_str = iStr.Size();
    uint64 len_bytes = iBlock.BytesTotal();
    uint64 len = FMath::Min( len_str, len_bytes / 2 );
    uint8* data = iBlock.Bits();
    for( uint64 i = 0; i < len; i += 2 ) {
        uint8 el = static_cast< uint8 >( str[ i / 2 ] );
        data[i] = ( data[i] & 0xFC ) | ( el & 0x3 );
        data[i+1] = ( data[i+1] & 0xFC ) | ( ( el & 0xC ) >> 2 );
    }
}

ULIS_NAMESPACE_END

