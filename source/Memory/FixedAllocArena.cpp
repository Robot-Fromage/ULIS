// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocMemoryPool.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FixedAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/FixedAllocArena.h"
#include "Memory/Memory.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
FFixedAllocArena::~FFixedAllocArena()
{
    ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    XFree( mBlock );
}

FFixedAllocArena::FFixedAllocArena(
      byte_t iArenaSize
    , byte_t iAllocSize
)
    : mArenaSize( iArenaSize )
    , mAllocSize( iAllocSize )
    , mBlock(
        reinterpret_cast< uint8* >( XMalloc( BlockSize() ) )
    )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mAllocSize > 0 && mAllocSize <= mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    InitializeCleanCellsMetaBase();
}

FFixedAllocArena::FFixedAllocArena(
      byte_t iAllocSize
    , uint32 iNumCells
)
    : mArenaSize(
          static_cast< uint64 >( iAllocSize )
        * static_cast< uint64 >( iNumCells )
    )
    , mAllocSize( iAllocSize )
    , mBlock(
        reinterpret_cast< uint8* >( XMalloc( BlockSize() ) )
    )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mAllocSize > 0 && mAllocSize <= mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    InitializeCleanCellsMetaBase();
}

bool
FFixedAllocArena::IsFull() const
{
    return  NumFreeCells() == 0;
}

bool
FFixedAllocArena::IsEmpty() const
{
    return  NumFreeCells() == NumCells();
}

bool
FFixedAllocArena::IsResident( tClient iClient ) const
{
    return  IsCellMetaBaseResident( ( *iClient ) - smMetaPadSize );
}

//static
bool
FFixedAllocArena::IsFree( tClient iClient )
{
    return  IsCellMetaBaseFree( ( *iClient ) - smMetaPadSize );
}

byte_t
FFixedAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

byte_t
FFixedAllocArena::AllocSize() const
{
    return  mAllocSize;
}

uint32
FFixedAllocArena::NumCells() const
{
    return
        static_cast< uint32 >(
              static_cast< uint64 >( mArenaSize )
            / static_cast< uint64 >( mAllocSize )
        );
}

uint32
FFixedAllocArena::NumFreeCells() const
{
    uint32 numFreeCells = 0;
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    for( tMetaBase metaBase = (tMetaBase)LowBlockAdress(); metaBase < highAdress; metaBase += cellPadding )
        if( IsCellMetaBaseFree( metaBase ) )
            numFreeCells++;

    return  numFreeCells;
}

uint32
FFixedAllocArena::NumUsedCells() const
{
    return  NumCells() - NumFreeCells();
}

const uint8*
FFixedAllocArena::LowBlockAdress() const
{
    return  mBlock;
}

const uint8*
FFixedAllocArena::HighBlockAdress() const
{
    return  mBlock + BlockSize();
}

tClient
FFixedAllocArena::Malloc()
{
    tMetaBase metaBase = FirstEmptyCellMetaBase();
    if( !metaBase )
        return  nullptr;

    tClient client = new tAlloc();
    *client = reinterpret_cast< tAlloc >( metaBase + smMetaPadSize );
    memcpy( metaBase, &client, smMetaPadSize );
    return  client;
}

// static
void
FFixedAllocArena::Free( tClient iClient )
{
#ifdef ULIS_ASSERT_ENABLED
    ULIS_ASSERT( iClient, "Cannot free null client." );
    ULIS_ASSERT( !IsFree( iClient ), "Cannot free already free client." );
    uint8* alloc = *iClient;
    ULIS_ASSERT( alloc, "Null alloc, corrupted state, bad client" );
#endif // ULIS_ASSERT_ENABLED

    memset( ( *iClient ) - smMetaPadSize, 0, smMetaPadSize );
    delete iClient;
}

void
FFixedAllocArena::UnsafeFreeAll()
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    for( tMetaBase metaBase = LowBlockAdress(); metaBase < highAdress; metaBase += cellPadding ) {
        if( !IsCellMetaBaseFree( metaBase ) ) {
            delete reinterpret_cast< tClient >( metaBase );
            memset( metaBase, 0, smMetaPadSize );
        }
    }
}

ufloat
FFixedAllocArena::OccupationRate() const
{
    return  static_cast< ufloat >( NumUsedCells() ) / NumCells();
}

void
FFixedAllocArena::DebugPrint() const
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    tMetaBase metaBase = (tMetaBase)LowBlockAdress();
    std::cout << "[";
    for( ; metaBase < highAdress; metaBase += cellPadding ) {
        if( IsCellMetaBaseFree( metaBase ) ) {
            std::cout << "-";
        } else {
            std::cout << "+";
        }
    }
    std::cout << "] " << FMath::CeilToInt( OccupationRate() * 100 ) <<"%\n";
}

bool
FFixedAllocArena::IsCellMetaBaseResident( tMetaBase iMetaBase ) const
{
    return  iMetaBase >= LowBlockAdress() && iMetaBase < HighBlockAdress();
}

//static
bool
FFixedAllocArena::IsCellMetaBaseFree( tMetaBase iMetaBase )
{
    ULIS_ASSERT( iMetaBase, "Bad input" );
    return  *reinterpret_cast< tClient* >( iMetaBase ) == nullptr;
}

uint8*
FFixedAllocArena::LowBlockAdress()
{
    return  mBlock;
}

uint8*
FFixedAllocArena::HighBlockAdress()
{
    return  mBlock + BlockSize();
}

uint64
FFixedAllocArena::BlockSize() const
{
    return  mArenaSize + static_cast< uint64 >( smMetaPadSize ) * static_cast< uint64 >( NumCells() );
}

uint64
FFixedAllocArena::CellPadding() const
{
    return  static_cast< uint64 >( mAllocSize ) + static_cast< uint64 >( smMetaPadSize );
}

tMetaBase
FFixedAllocArena::FirstEmptyCellMetaBase( tMetaBase iFromMetaBase, tMetaBase* oFoundMetaBase )
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    tMetaBase metaBase = iFromMetaBase ? iFromMetaBase : LowBlockAdress();
    ULIS_ASSERT( IsCellMetaBaseResident( metaBase ), "Not Resident From" );
    for( ; metaBase < highAdress; metaBase += cellPadding ) {
        if( IsCellMetaBaseFree( metaBase ) ) {
            if( oFoundMetaBase )
                *oFoundMetaBase = metaBase;
            return  metaBase;
        }
    }
    return  nullptr;
}

tMetaBase
FFixedAllocArena::FirstFullCellMetaBase( tMetaBase iFromMetaBase, tMetaBase* oFoundMetaBase )
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    tMetaBase metaBase = iFromMetaBase ? iFromMetaBase : LowBlockAdress();
    ULIS_ASSERT( IsCellMetaBaseResident( metaBase ), "Not Resident From" );
    for( ; metaBase < highAdress; metaBase += cellPadding ) {
        if( !IsCellMetaBaseFree( metaBase ) ) {
            if( oFoundMetaBase )
                *oFoundMetaBase = metaBase;
            return  metaBase;
        }
    }
    return  nullptr;
}

//static
void
FFixedAllocArena::MoveAlloc( tMetaBase iSrcMetaBase, tMetaBase iDstMetaBase, byte_t iAllocSize )
{
    ULIS_ASSERT( !IsCellMetaBaseFree( iSrcMetaBase ), "Bad move, source should not be free !" );
    ULIS_ASSERT( IsCellMetaBaseFree( iDstMetaBase ), "Bad move, destination should be free !" );
    memcpy( iDstMetaBase + smMetaPadSize, iSrcMetaBase + smMetaPadSize, iAllocSize );
    memset( iSrcMetaBase, 0, smMetaPadSize );
    tClient client = reinterpret_cast< tClient >( iDstMetaBase );
    *client = reinterpret_cast< tAlloc >( iDstMetaBase + smMetaPadSize );
}

void
FFixedAllocArena::InitializeCleanCellsMetaBase()
{
    uint8 const * const highAdress = HighBlockAdress();
    const uint64 cellPadding = CellPadding();
    for( tMetaBase metaBase = LowBlockAdress(); metaBase < highAdress; metaBase += cellPadding )
        memset( metaBase, 0, smMetaPadSize );
}

ULIS_NAMESPACE_END

