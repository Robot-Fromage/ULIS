// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ClearRaw.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the ClearRaw functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Clear/ClearRaw.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
void ClearRaw( FBlock& iDst, bool iCallDirty ) {
    memset( iDst.Bits(), 0, iDst.BytesTotal() );
    iDst.Dirty( iCallDirty );
}
ULIS_NAMESPACE_END

