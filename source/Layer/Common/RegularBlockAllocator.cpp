// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RegularBlockAllocator.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FRegularBlockAllocator class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Common/RegularBlockAllocator.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
//static
FBlock*
FRegularBlockAllocator::New( uint16 iWidth, uint16 iHeight, eFormat iFormat, const FColorSpace* iColorSpace )
{
    return  new FBlock( iWidth, iHeight, iFormat, iColorSpace );
}

//static
void
FRegularBlockAllocator::Delete( FBlock* iBlock )
{
    delete  iBlock;
}
ULIS_NAMESPACE_END

