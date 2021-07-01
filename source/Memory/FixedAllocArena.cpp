// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocArena.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FixedAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/FixedAllocArena.h"
#include "Memory/Memory.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
// Static Meta Spec
static constexpr const uint8 sgMetaPadSize = sizeof( tClient ); ///< Constant padding for meta base storage.

FFixedAllocArena::FIterator::FIterator(
      tMetaBase iMetaBase
    , FFixedAllocArena const * const iArena
)
    : mMetaBase( iMetaBase )
    , mArena( iArena )
{}

FFixedAllocArena::FIterator::FIterator(
      tClient iClient
    , FFixedAllocArena const * const iArena
)
    : mMetaBase( ( *iClient ) - sgMetaPadSize )
    , mArena( iArena )
{}

//static
FFixedAllocArena::FIterator
FFixedAllocArena::FIterator::MakeNull() {
    return  FIterator( tMetaBase( nullptr ), nullptr );
}

FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator++() {
    mMetaBase += mArena->CellSize();
    return  *this;
}

FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator--() {
    mMetaBase -= mArena->CellSize();
    return  *this;
}

const FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator++() const {
    mMetaBase += mArena->CellSize();
    return  *this;
}

const FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator--() const {
    mMetaBase -= mArena->CellSize();
    return  *this;
}

bool
FFixedAllocArena::FIterator::operator==( const FIterator& iOther ) const {
    return  mMetaBase == iOther.mMetaBase;
}

bool
FFixedAllocArena::FIterator::operator!=( const FIterator& iOther ) const {
    return  mMetaBase != iOther.mMetaBase;
}

tClient
FFixedAllocArena::FIterator::Client() {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    ULIS_ASSERT( IsResident(), "dereferencing non resident metabase" )
    return  *( tClient* )( mMetaBase );
}

const tClient
FFixedAllocArena::FIterator::Client() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    ULIS_ASSERT( IsResident(), "dereferencing non resident metabase" )
    return  *( const tClient* )( mMetaBase );
}

tMetaBase
FFixedAllocArena::FIterator::MetaBase() {
    return  mMetaBase;
}

const tMetaBase
FFixedAllocArena::FIterator::MetaBase() const {
    return  mMetaBase;
}

tAlloc
FFixedAllocArena::FIterator::Allocation() {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    ULIS_ASSERT( IsResident(), "dereferencing non resident metabase" )
    return  ( tAlloc )( mMetaBase + sgMetaPadSize );
}

const tAlloc
FFixedAllocArena::FIterator::Allocation() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    ULIS_ASSERT( IsResident(), "dereferencing non resident metabase" )
    return  ( tAlloc )( mMetaBase + sgMetaPadSize );
}

bool
FFixedAllocArena::FIterator::IsFree() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    ULIS_ASSERT( IsResident(), "dereferencing non resident metabase" )
    return  *( tClient* )( mMetaBase ) == nullptr;
}

bool
FFixedAllocArena::FIterator::IsUsed() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    ULIS_ASSERT( IsResident(), "dereferencing non resident metabase" )
    return  *( tClient* )( mMetaBase ) != nullptr;
}

bool
FFixedAllocArena::FIterator::IsValid() const {
    return  mMetaBase != nullptr;
}

bool
FFixedAllocArena::FIterator::IsResident() const {
    return  mMetaBase >= mArena->LowBlockAdress() && mMetaBase < mArena->HighBlockAdress();
}

uint64
FFixedAllocArena::FIterator::AllocSize() const {
    return  mArena->AllocSize();
}

uint64
FFixedAllocArena::FIterator::CellSize() const {
    return  mArena->CellSize();
}

void
FFixedAllocArena::FIterator::CleanupMetaBase() {
    memset( mMetaBase, 0, sgMetaPadSize );
}

void
FFixedAllocArena::FIterator::FreeClient() {
    delete  Client();
}

void
FFixedAllocArena::FIterator::ResyncClient() {
    *Client() = Allocation();
}

tClient
FFixedAllocArena::FIterator::AllocClient() {
    return  IsValid() ? *(tClient*)( mMetaBase ) = new tAlloc( Allocation() ) : nullptr;
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
    , mBlock( ( uint8* )( XMalloc( BlockSize() ) ) )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mAllocSize > 0 && mAllocSize <= mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    Initialize();
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
    , mBlock( ( uint8* )( XMalloc( BlockSize() ) ) )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mAllocSize > 0 && mAllocSize <= mArenaSize, "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    Initialize();
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
    return  uint32( mArenaSize / mAllocSize );
}

uint32
FFixedAllocArena::NumFreeCells() const
{
    uint32 numFreeCells = 0;
    const FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        numFreeCells += it.IsFree();
        ++it;
    }

    return  numFreeCells;
}

uint32
FFixedAllocArena::NumUsedCells() const
{
    return  NumCells() - NumFreeCells();
}

tClient
FFixedAllocArena::Malloc()
{
    FIterator it = FindFirst( false );
    return  it.AllocClient();
}

// static
void
FFixedAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client." );
    FIterator it( iClient, nullptr );
    it.FreeClient();
    it.CleanupMetaBase();
}

void
FFixedAllocArena::UnsafeFreeAll()
{
    FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        it.FreeClient();
        it.CleanupMetaBase();
        ++it;
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
    FIterator it = Begin();
    const FIterator end = End();
    char c[2] = { '+', '-' };
    std::cout << "[";
    while( it != end ) {
        std::cout << c[ it.IsFree() ];
        ++it;
    }
    std::cout << "]";
    std::cout << " " << FMath::FloorToInt( OccupationRate() * 100 ) <<"%\n";
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

uint64
FFixedAllocArena::BlockSize() const
{
    return  mArenaSize + static_cast< uint64 >( sgMetaPadSize ) * static_cast< uint64 >( NumCells() );
}

uint64
FFixedAllocArena::CellSize() const {
    return  mAllocSize + uint64( sgMetaPadSize );
}

//static
void
FFixedAllocArena::MoveAlloc( FIterator& iFrom, FIterator& iTo )
{
    ULIS_ASSERT( iFrom.IsUsed(), "Bad move, source should be used !" );
    ULIS_ASSERT( iTo.IsFree(), "Bad move, destination should be free !" );
    ULIS_ASSERT( iFrom.CellSize() == iTo.CellSize(), "Bad move, cells should be in arenas with equivalent cell sizes" );
    memcpy( iTo.MetaBase(), iFrom.MetaBase(), iFrom.CellSize() );
    iFrom.CleanupMetaBase();
    iTo.ResyncClient();
}

FFixedAllocArena::FIterator
FFixedAllocArena::FindFirst( bool iUsed, const FIterator& iFrom )
{
    FIterator it = iFrom.IsValid() ? iFrom : Begin();
    ULIS_ASSERT( it.IsResident(), "Non resident from param" );
    const FIterator end = End();
    while( it != end ) {
        if( it.IsFree() == iUsed )
            return  it;
        ++it;
    }
    return  FIterator::MakeNull();
}

void
FFixedAllocArena::Initialize()
{
    FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        memset( it.MetaBase(), 0, sgMetaPadSize );
        ++it;
    }
}

FFixedAllocArena::FIterator
FFixedAllocArena::Begin() {
    // reinterpret_cast
    return  FIterator( ( tMetaBase* )LowBlockAdress(), this );
}

FFixedAllocArena::FIterator
FFixedAllocArena::End() {
    // const_cast
    return  FIterator( ( tMetaBase* )HighBlockAdress(), this );
}

const FFixedAllocArena::FIterator
FFixedAllocArena::Begin() const {
    // const_cast + reinterpret_cast
    return  FIterator( ( tMetaBase* )LowBlockAdress(), this );
}

const FFixedAllocArena::FIterator
FFixedAllocArena::End() const {
    // const_cast + reinterpret_cast
    return  FIterator( ( tMetaBase* )HighBlockAdress(), this );
}

ULIS_NAMESPACE_END

