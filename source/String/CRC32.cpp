// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CRC32.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for CRC32 function.
* @license      Please refer to LICENSE.md
*/
#include "String/CRC32.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// CRC32
uint32
CRC32( const uint8* iData, int iLen )
{
    int i = 0;
    int j = 0;
    unsigned int byte = 0;
    unsigned int crc = 0;
    unsigned int mask = 0;

    i = 0;
    crc = 0xFFFFFFFF;
    while( i < iLen )
    {
        byte = iData[i];
        crc = crc ^ byte;
        for( j = 7; j >= 0; j-- )
        {
            #pragma warning(push)
            #pragma warning(disable : 4146)
            mask = static_cast< unsigned int >( -( crc & 1 ) );
            crc = ( crc >> 1 ) ^ ( 0xEDB88320 & mask );
            #pragma warning(pop)
        }
        i = i + 1;
    }

    return  ~crc;
}

uint32 CRC32( const char* iData ) {
    return  CRC32( reinterpret_cast< const uint8* >( iData ), static_cast< int >( strlen( iData ) ) );
}

ULIS_NAMESPACE_END

