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
FFixedAllocArena::FIterator::FIterator( tMetaBase* iMetaBase );
FFixedAllocArena::FIterator::FIterator( tClient* iClient );
FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator++();
FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator--();
uint32_t
FFixedAllocArena::FIterator::PrevSize() const {
    return  *( uint32* )( mMetaBase + smMetaClientPadSize );
}

uint32_t
FFixedAllocArena::FIterator::NextSize() const {
    return  *( uint32* )( mMetaBase + smMetaClientPadSize );
}

tClient
FFixedAllocArena::FIterator::Client() const {
    return  *( tClient* )( mMetaBase );
}

void
FFixedAllocArena::FIterator::SetPrevSize( uint32 iSize ) {
    *( uint32* )( mMetaBase + smMetaClientPadSize ) = iSize;
}

void
FFixedAllocArena::FIterator::SetNextSize( uint32 iSize ) {
    *( uint32* )( mMetaBase + smMetaClientPadSize ) = iSize;
}

void
FFixedAllocArena::FIterator::SetClient( tClient iClient ) {
    *( iClient* )( mMetaBase ) = iClient;
}

bool
FFixedAllocArena::FIterator::HasReachedEndSentinel() const {
    return  NextSize() == 0;
}

bool
FFixedAllocArena::FIterator::HasReachedBeginSentinel() const {
    return  PrevSize() == 0;
}

bool
FFixedAllocArena::FIterator::IsFree() const {
    return  *( iClient* )( mMetaBase ) == nullptr;
}

bool
FFixedAllocArena::FIterator::IsUsed() const {
    return  *( iClient* )( mMetaBase ) != nullptr;
}

tData
FFixedAllocArena::FIterator::Allocation() {
    return  IsUsed() ? *iClient : nullptr;
}

const tData Allocation() const {
    return  IsUsed() ? *iClient : nullptr;
}

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

byte_t
FFixedAllocArena::ArenaSize() const
{
    return  byte_t( static_cast< double >( mArenaSize ) );
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
    uint8 const * const hi = HighBlockAdress();
    uint64 const cpad = CellPadding();
    for( tMetaBase metaBase = (tMetaBase)LowBlockAdress(); metaBase < hi; metaBase += cpad )
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
    return  metaBase ? *(tClient*)metaBase = new tAlloc( metaBase + smMetaPadSize ) : nullptr;
}

// static
void
FFixedAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client." );
    memset( ( *iClient ) - smMetaPadSize, 0, smMetaPadSize );
    delete iClient;
}

void
FFixedAllocArena::UnsafeFreeAll()
{
    uint8 const * const hi = HighBlockAdress();
    uint64 const cpad = CellPadding();
    for( tMetaBase metaBase = LowBlockAdress(); metaBase < hi; metaBase += cpad ) {
        if( !IsCellMetaBaseFree( metaBase ) ) {
            delete  *(tClient*)metaBase;
            memset( metaBase, 0, smMetaPadSize );
        }
    }
    // simon
    // canada
}

ufloat
FFixedAllocArena::OccupationRate() const
{
    return  static_cast< ufloat >( NumUsedCells() ) / NumCells();
}

void
FFixedAllocArena::DebugPrint() const
{
    uint8 const * const hi = HighBlockAdress();
    uint64 const cpad = CellPadding();
    tMetaBase metaBase = (tMetaBase)LowBlockAdress();
    std::cout << "[";
    for( ; metaBase < hi; metaBase += cpad ) {
        if( IsCellMetaBaseFree( metaBase ) ) {
            std::cout << "-";
        } else {
            std::cout << "+";
        }
    }
    std::cout << "] " << FMath::FloorToInt( OccupationRate() * 100 ) <<"%\n";
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
FFixedAllocArena::FirstEmptyCellMetaBase( tMetaBase iFromMetaBase )
{
    uint8 const * const hi = HighBlockAdress();
    uint64 const cpad = CellPadding();
    tMetaBase metaBase = iFromMetaBase ? iFromMetaBase : LowBlockAdress();
    ULIS_ASSERT( IsCellMetaBaseResident( metaBase ), "Not Resident From" );
    for(; metaBase < hi; metaBase += cpad )
        if( IsCellMetaBaseFree( metaBase ) )
            return  metaBase;
    return  nullptr;
}

tMetaBase
FFixedAllocArena::FirstFullCellMetaBase( tMetaBase iFromMetaBase )
{
    uint8 const * const hi = HighBlockAdress();
    uint64 const cpad = CellPadding();
    tMetaBase metaBase = iFromMetaBase ? iFromMetaBase : LowBlockAdress();
    ULIS_ASSERT( IsCellMetaBaseResident( metaBase ), "Not Resident From" );
    for(; metaBase < hi; metaBase += cpad )
        if( !IsCellMetaBaseFree( metaBase ) )
            return  metaBase;
    return  nullptr;
}

//static
void
FFixedAllocArena::MoveAlloc( tMetaBase iSrcMetaBase, tMetaBase iDstMetaBase, byte_t iAllocSize )
{
    ULIS_ASSERT( !IsCellMetaBaseFree( iSrcMetaBase ), "Bad move, source should not be free !" );
    ULIS_ASSERT( IsCellMetaBaseFree( iDstMetaBase ), "Bad move, destination should be free !" );
    memcpy( iDstMetaBase, iSrcMetaBase, static_cast< uint64 >( iAllocSize ) + static_cast< uint64 >( smMetaPadSize ) );
    memset( iSrcMetaBase, 0, smMetaPadSize );
    **(tClient*)iDstMetaBase = iDstMetaBase + smMetaPadSize;
}

void
FFixedAllocArena::InitializeCleanCellsMetaBase()
{
    uint8 const * const hi = HighBlockAdress();
    uint64 const cpad = CellPadding();
    for( tMetaBase metaBase = LowBlockAdress(); metaBase < hi; metaBase += cpad )
        memset( metaBase, 0, smMetaPadSize );
}

ULIS_NAMESPACE_END

