// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         FormatInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FormatInfo utilities.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Data/FormatInfo.h"

/////////////////////////////////////////////////////
// FFormatInfo
ULIS2_NAMESPACE_BEGIN
FFormatInfo::~FFormatInfo() {
    delete [] IDT;
}

FFormatInfo::FFormatInfo( tFormat iFMT )
    : FMT( iFMT )
    , IDT( nullptr )
{
    BPC = ULIS2_R_DEPTH(    FMT );
    NCC = ULIS2_R_CHANNELS( FMT );
    HEA = ULIS2_R_ALPHA(    FMT );
    COD = ULIS2_R_RS(       FMT );
    SPP = NCC + HEA;
    BPP = SPP * BPC;
    IDT = new uint8[ SPP ];
    CM = static_cast< eColorModel >( ULIS2_R_MODEL( FMT ) );
    TP = static_cast< eType >( ULIS2_R_TYPE( FMT ) );
    SWA = ULIS2_R_SWAP( FMT );
    REV = ULIS2_R_REVERSE( FMT );

    uint8 msp = SPP - 1;
    switch( COD ) {
        case 1:  for( int i = 0; i < SPP; ++i ) IDT[i] = ( msp - i );                                 AID = 0;   break;
        case 2:  for( int i = 0; i < SPP; ++i ) IDT[i] = ( i + 1 ) > msp ? 0 : i + 1;                 AID = 0;   break;
        case 3:  for( int i = 0; i < SPP; ++i ) IDT[i] = ( msp - i ) - 1 < 0 ? msp : ( msp - i ) - 1; AID = msp; break;
        default: for( int i = 0; i < SPP; ++i ) IDT[i] = i;                                           AID = msp; break;
    }
}

ULIS2_NAMESPACE_END

