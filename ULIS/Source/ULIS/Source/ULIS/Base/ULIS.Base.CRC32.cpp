// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file       ULIS.Base.CRC32.cpp
 * @author     Clement Berthaud
 * @copyright  Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license    Please refer to LICENSE.md
 * @brief      This file provides the definition for the CRC32 function.
 */
#include "ULIS/Base/ULIS.Base.CRC32.h"

namespace ULIS {
/////////////////////////////////////////////////////
// CRC32
uint32
CRC32( const uint8*iData, int iLen )
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
         mask = -( crc & 1 );
         crc = ( crc >> 1 ) ^ ( 0xEDB88320 & mask );
      }
      i = i + 1;
   }

   return  ~crc;
}

} // namespace ULIS

