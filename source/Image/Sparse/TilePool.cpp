// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TilePool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TilePool class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Sparse/TilePool.h"

#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
#include "Image/Sparse/TilePool_Private.h"
#endif

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// TTilePool
//static
ITilePool*
ITilePool::XCreateTilePool( eFormat iFormat, FColorSpace* iProfile, eMicro iMicro, eMacro iMacro ) {
    /*
    #ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
        return  new  TTilePool< 1, 5 >( iFormat, iProfile );
    #else
        return  nullptr;
    #endif
    */
    return  nullptr;
}

//static
void
ITilePool::XDeleteTilePool( ITilePool* iTilePool ) {
    delete  iTilePool;
}
ULIS_NAMESPACE_END
