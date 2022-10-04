// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FixedAllocArena.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for FixedAllocArena.
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
    , uint64 iCellSize
)
    : mMetaBase( iMetaBase )
    , mCellSize( iCellSize )
{}

FFixedAllocArena::FIterator::FIterator(
      tClient iClient
    , uint64 iCellSize
)
    : mMetaBase( ( *iClient ) - sgMetaPadSize )
    , mCellSize( iCellSize )
{}

FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator=( const FIterator& iOther ) {
    mMetaBase = iOther.mMetaBase;
    mCellSize = iOther.mCellSize;
    return  *this;
}

//static
FFixedAllocArena::FIterator
FFixedAllocArena::FIterator::MakeNull() {
    return  FIterator( tMetaBase( nullptr ), 0 );
}

FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator++() {
    mMetaBase += mCellSize;
    return  *this;
}

FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator--() {
    mMetaBase -= mCellSize;
    return  *this;
}

const FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator++() const {
    mMetaBase += mCellSize;
    return  *this;
}

const FFixedAllocArena::FIterator&
FFixedAllocArena::FIterator::operator--() const {
    mMetaBase -= mCellSize;
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
    return  *( tClient* )( mMetaBase );
}

const tClient
FFixedAllocArena::FIterator::Client() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
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
    return  ( tAlloc )( mMetaBase + sgMetaPadSize );
}

const tAlloc
FFixedAllocArena::FIterator::Allocation() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  ( tAlloc )( mMetaBase + sgMetaPadSize );
}

bool
FFixedAllocArena::FIterator::IsFree() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  *( tClient* )( mMetaBase ) == nullptr;
}

bool
FFixedAllocArena::FIterator::IsUsed() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  *( tClient* )( mMetaBase ) != nullptr;
}

bool
FFixedAllocArena::FIterator::IsValid() const {
    return  mMetaBase != nullptr;
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

uint64
FFixedAllocArena::FIterator::CellSize() const {
    return  mCellSize;
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
    , uint64 iNumCells
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
    return  mArenaSize;
}

byte_t
FFixedAllocArena::AllocSize() const
{
    return  mAllocSize;
}

uint64
FFixedAllocArena::NumCells() const
{
    return  mArenaSize / mAllocSize;
}

uint64
FFixedAllocArena::NumFreeCells() const
{
    uint64 numFreeCells = 0;
    const FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        numFreeCells += it.IsFree();
        ++it;
    }

    return  numFreeCells;
}

uint64
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
    FIterator it( iClient, 0 );
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

bool
FFixedAllocArena::IsResident( tMetaBase iMetaBase ) {
    return  iMetaBase >= LowBlockAdress() && iMetaBase < HighBlockAdress();
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
    ULIS_ASSERT( IsResident( it.MetaBase() ), "Non resident from param" );
    const FIterator end = End();
    while( it != end ) {
        if( it.IsUsed() == iUsed )
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
    return  FIterator( ( tMetaBase )LowBlockAdress(), CellSize() );
}

FFixedAllocArena::FIterator
FFixedAllocArena::End() {
    // const_cast
    return  FIterator( ( tMetaBase )HighBlockAdress(), CellSize() );
}

const FFixedAllocArena::FIterator
FFixedAllocArena::Begin() const {
    // const_cast + reinterpret_cast
    return  FIterator( ( tMetaBase )LowBlockAdress(), CellSize() );
}

const FFixedAllocArena::FIterator
FFixedAllocArena::End() const {
    // const_cast + reinterpret_cast
    return  FIterator( ( tMetaBase )HighBlockAdress(), CellSize() );
}

ULIS_NAMESPACE_END

