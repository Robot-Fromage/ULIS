// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ShrinkableAllocArena.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for ShrinkableAllocArena.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/ShrinkableAllocArena.h"
#include "Memory/Memory.h"
#include "Math/Math.h"
#include "Core/Constants.h"

ULIS_NAMESPACE_BEGIN
static constexpr const uint8 sgMetaPrevDeltaPad = sizeof( tClient ); ///< Constant padding for meta base storage: prev delta pad.
static constexpr const uint8 sgMetaNextDeltaPad = sgMetaPrevDeltaPad + sizeof( uint32 ); ///< Constant padding for meta base storage: next delta pad.
static constexpr const uint8 sgMetaTotalPad = sgMetaNextDeltaPad + sizeof( uint32 ); ///< Constant padding for meta base storag: total pad.
static constexpr char sgBlockChar = char( 219 );

FShrinkableAllocArena::FIterator::FIterator(
    tMetaBase iMetaBase
)
    : mMetaBase( iMetaBase )
{}

FShrinkableAllocArena::FIterator::FIterator(
    tClient iClient
)
    : mMetaBase( ( *iClient ) - sgMetaTotalPad )
{}

FShrinkableAllocArena::FIterator&
FShrinkableAllocArena::FIterator::operator=( const FIterator& iOther ) {
    mMetaBase = iOther.mMetaBase;
}

//static
FShrinkableAllocArena::FIterator
FShrinkableAllocArena::FIterator::MakeNull() {
    return  FIterator( tMetaBase( nullptr ) );
}

FShrinkableAllocArena::FIterator&
FShrinkableAllocArena::FIterator::operator++() {
    ULIS_ASSERT( !IsEnd(), "Bad" );
    mMetaBase += sgMetaTotalPad + NextSize();
    return  *this;
}

FShrinkableAllocArena::FIterator&
FShrinkableAllocArena::FIterator::operator--() {
    ULIS_ASSERT( !IsBegin(), "Bad" );
    mMetaBase -= sgMetaTotalPad + PrevSize();
    return  *this;
}

const FShrinkableAllocArena::FIterator&
FShrinkableAllocArena::FIterator::operator++() const {
    ULIS_ASSERT( !IsEnd(), "Bad" );
    mMetaBase += sgMetaTotalPad + NextSize();
    return  *this;
}

const FShrinkableAllocArena::FIterator&
FShrinkableAllocArena::FIterator::operator--() const {
    ULIS_ASSERT( !IsBegin(), "Bad" );
    mMetaBase -= sgMetaTotalPad + PrevSize();
    return  *this;
}

FShrinkableAllocArena::FIterator
FShrinkableAllocArena::FIterator::operator+( const FIterator& iT, uint64 iValue ) {
    FIterator it( iT );
    for( uint64 i = 0; i < iValue; ++i )
        ++it;
    return it;
}

FShrinkableAllocArena::FIterator
FShrinkableAllocArena::FIterator::operator-( const FIterator& iT, uint64 iValue ) {
    FIterator it( iT );
    for( uint64 i = 0; i < iValue; ++i )
        --it;
    return it;
}

bool
FShrinkableAllocArena::FIterator::operator==( const FIterator& iOther ) const {
    return  mMetaBase == iOther.mMetaBase;
}

bool
FShrinkableAllocArena::FIterator::operator!=( const FIterator& iOther ) const {
    return  mMetaBase != iOther.mMetaBase;
}

uint32
FShrinkableAllocArena::FIterator::PrevSize() const {
    return  *(uint32*)( mMetaBase + sgMetaPrevDeltaPad );
}

uint32
FShrinkableAllocArena::FIterator::NextSize() const {
    return  *(uint32*)( mMetaBase + sgMetaNextDeltaPad );
}

tClient
FShrinkableAllocArena::FIterator::Client() {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  *( tClient* )( mMetaBase );
}

const tClient
FShrinkableAllocArena::FIterator::Client() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  *( tClient* )( mMetaBase );
}

tMetaBase
FShrinkableAllocArena::FIterator::MetaBase() {
    return  mMetaBase;
}
const tMetaBase
FShrinkableAllocArena::FIterator::MetaBase() const {
    return  mMetaBase;
}

tAlloc
FShrinkableAllocArena::FIterator::Allocation() {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  ( tAlloc )( mMetaBase + sgMetaTotalPad );
}

const tAlloc
FShrinkableAllocArena::FIterator::Allocation() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  ( tAlloc )( mMetaBase + sgMetaTotalPad );
}
bool
FShrinkableAllocArena::FIterator::IsFree() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  *( tClient* )( mMetaBase ) == nullptr;
}

bool
FShrinkableAllocArena::FIterator::IsUsed() const {
    ULIS_ASSERT( IsValid(), "dereferencing invalid metabase" )
    return  *( tClient* )( mMetaBase ) != nullptr;
}

bool
FShrinkableAllocArena::FIterator::IsValid() const {
    return  ( mMetaBase != nullptr ) && ( *( uint64* )( mMetaBase ) != ULIS_UINT64_MAX );
}

bool
FShrinkableAllocArena::FIterator::IsBegin() const {
    return  PrevSize() == 0;
}

bool
FShrinkableAllocArena::FIterator::IsEnd() const {
    return  NextSize() == 0;
}

void
FShrinkableAllocArena::FIterator::CleanupMetaBase() {
    memset( mMetaBase, 0, sgMetaTotalPad );
}

void
FShrinkableAllocArena::FIterator::FillMetaBase() {
    // Should not register as a free alloc.
    *( uint64* )( mMetaBase ) = ULIS_UINT64_MAX;
}

void
FShrinkableAllocArena::FIterator::SetPrevSize( uint32 iValue ) {
    *( uint32* )( mMetaBase + sgMetaPrevDeltaPad ) = iValue;
}

void
FShrinkableAllocArena::FIterator::SetNextSize( uint32 iValue ) {
    *( uint32* )( mMetaBase + sgMetaNextDeltaPad ) = iValue;
}

void
FShrinkableAllocArena::FIterator::FreeClient() {
    delete  Client();
}

void
FShrinkableAllocArena::FIterator::ResyncClient() {
    *Client() = Allocation();
}

tClient
FShrinkableAllocArena::FIterator::AllocClient() {
    ULIS_ASSERT( IsValid() && IsFree(), "Bad" );
    return  IsValid() ? *(tClient*)( mMetaBase ) = new tAlloc( Allocation() ) : nullptr;
}

//static
void
FShrinkableAllocArena::FIterator::MergeFree( const FIterator& iA, const FIterator& iB ) {
    ULIS_ASSERT( iA + 1 == iB, "Bad" );
    if( iA.IsUsed() || iB.IsUsed() )
        return;

    FIterator n0 = iA;
    FIterator n1 = iB + 1;
    uint64 mergeSize = n0.NextSize() + sgMetaTotalPad + n1.PrevSize();
    n0.SetNextSize( mergeSize );
    n1.SetPrevSize( mergeSize );
}

FShrinkableAllocArena::~FShrinkableAllocArena()
{
    ULIS_ASSERT( IsEmpty(), "Error, trying to delete a non empty arena !" );
    XFree( mBlock );
}

FShrinkableAllocArena::FShrinkableAllocArena(
      byte_t iArenaSize
    , byte_t iMaxAllocSize
)
    : mArenaSize( iArenaSize )
    , mMaxAllocSize( iMaxAllocSize )
    , mBlock( ( uint8* )( XMalloc( BlockSize() ) ) )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize > 0, "Bad Size !" );
    ULIS_ASSERT( uint64( mMaxAllocSize ) + uint64( sgMetaTotalPad ) < uint64( mArenaSize ), "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    Initialize();
}

FShrinkableAllocArena::FShrinkableAllocArena(
      byte_t iMaxAllocSize
    , uint64 iNumCells
)
    : mArenaSize(
        // Room for allocs
          uint64( iMaxAllocSize )
        * uint64( iNumCells )
        // Room for meta Bases, including begin sentinel
        + uint64( sgMetaTotalPad )
        * uint64( iNumCells )
        // Room for end sentinel
        + uint64( sgMetaTotalPad )
    )
    , mMaxAllocSize( iMaxAllocSize )
    , mBlock( ( uint8* )( XMalloc( BlockSize() ) ) )
{
    ULIS_ASSERT( mArenaSize > 0, "Bad Size !" );
    ULIS_ASSERT( mMaxAllocSize > 0, "Bad Size !" );
    ULIS_ASSERT( uint64( mMaxAllocSize ) + uint64( sgMetaTotalPad ) < uint64( mArenaSize ), "Bad Size !" );
    ULIS_ASSERT( mBlock, "Bad Alloc !" );
    Initialize();
}

bool
FShrinkableAllocArena::IsFull() const
{
    return  uint64( FreeMemory() ) == 0;
}

bool
FShrinkableAllocArena::IsEmpty() const
{
    FIterator begin = Begin();
    uint64 initialFreeBufferSize = uint64( mArenaSize ) - uint64( sgMetaTotalPad ) * 2;
    return begin.IsFree() && begin.NextSize() == initialFreeBufferSize;
}

byte_t
FShrinkableAllocArena::ArenaSize() const
{
    return  mArenaSize;
}

byte_t
FShrinkableAllocArena::MaxAllocSize() const
{
    return  mMaxAllocSize;
}

byte_t
FShrinkableAllocArena::TotalMemory() const
{
    return  ArenaSize();
}

byte_t
FShrinkableAllocArena::FreeMemory() const
{
    uint64 free = 0;
    const FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        if( it.IsFree() )
            free += it.NextSize();
        ++it;
    }

    return  free;
}

byte_t
FShrinkableAllocArena::UsedMemory() const
{
    return  TotalMemory() - FreeMemory();
}

tClient
FShrinkableAllocArena::Malloc( byte_t iSize )
{
    uint64 req = FMath::Min( uint64( MaxAllocSize() ), uint64( iSize ) );
    FIterator it = FindFirstMinAlloc( false, req );

    if( !it.IsValid() )
        return  nullptr;

    it.AllocClient();
    uint64 chunkSize = it.NextSize();
    uint64 rem = chunkSize > req + sgMetaTotalPad ? chunkSize - ( req + sgMetaTotalPad ) : 0;

    if( rem == 0 )
        return it.Client();

    it.SetNextSize( req );
    FIterator sec = it + 1;
    sec.CleanupMetaBase();
    sec.SetPrevSize( req );
    sec.SetNextSize( rem );
    FIterator::MergeFree( sec, sec + 1 );

    return it.Client();
}

//static
void
FShrinkableAllocArena::Free( tClient iClient )
{
    ULIS_ASSERT( iClient, "Cannot free null client" );
    FIterator it( iClient );
    it.FreeClient();
    it.CleanupMetaBase();
    FIterator::MergeFree( it, it + 1 );
    FIterator::MergeFree( it - 1, it );
}

void
FShrinkableAllocArena::UnsafeFreeAll() {
    FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        it.FreeClient();
        it.CleanupMetaBase();
        ++it;
    }
    Initialize();
}

float
FShrinkableAllocArena::LocalFragmentation() const
{
    uint32 runFree, maxFree, numFree, runUsed, maxUsed, totalFree, totalUsed;
    runFree = maxFree = numFree = runUsed = maxUsed = totalFree = totalUsed = 0;
    const FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        if( it.IsFree() ) {
            runFree += size;
            totalFree += size;
            runUsed = 0;
            numFree++;
        } else {
            runUsed += size + static_cast< uint64 >( sgMetaTotalPad );
            totalUsed += size + static_cast< uint64 >( sgMetaTotalPad );
            runFree = 0;
        }
        maxFree = FMath::Max( runFree, maxFree );
        maxUsed = FMath::Max( runUsed, maxUsed );
        ++it;
    }
    maxUsed += numFree * sgMetaTotalPad;
    totalUsed += numFree * sgMetaTotalPad;

    if( totalFree == 0 )
        return  0.f;

    float frag = ( totalFree - maxFree ) / static_cast< float >( totalFree );
    float pack = ( totalUsed - maxUsed ) / static_cast< float >( totalUsed );
    return  ( frag + pack ) / 2;
}

void
FShrinkableAllocArena::DefragSelfForce()
{
    // Align all allocs to the left of the buffer.
    // Use memmove to avoid overlapping self sector.
    // pass contiguous blocks.
    // first find a src ( first empty after contiguous used ).
    // then find a dst ( first [next] used after src. ).
    tMetaBase metaBase = mBlock;
    tMetaBase src = nullptr;
    tMetaBase dst = nullptr;
    while( IsMetaBaseResident( metaBase ) ) {
        bool bFree = IsMetaBaseFree( metaBase );
        if( bFree ) {
            dst = metaBase;
            src = metaBase;
            uint32 dstChunkSize = *(uint32*)( dst + smMetaClientPadSize + smMetaPrevDeltaPadSize );
            uint32 ctgRun = 0;
            while( IsMetaBaseResident( src ) ) {
                ctgRun += ( MetaBaseSize( src ) ) + static_cast< uint32 >( sgMetaTotalPad );
                src = NextMetaBase( src );
            }
            memmove( dst, src, ctgRun + sgMetaTotalPad );
            *(uint32*)( src + smMetaClientPadSize + smMetaPrevDeltaPadSize ) += dstChunkSize;
            metaBase = src;
        } else {
            metaBase = NextMetaBase( metaBase );
        }
    }
}

void
FShrinkableAllocArena::DebugPrint( bool iShort, int iCol ) const
{
    if( iShort )
    {
        // Print short version, scaled down with rough estimate of sector status.
        std::cout << "[";
        uint8* raw_buf = new uint8[mArenaSize];
        int index = 0;
        tMetaBase metaBase = mBlock;
        while( IsMetaBaseResident( metaBase ) ) {
            bool bFree = IsMetaBaseFree( metaBase );
            uint32 size = ( MetaBaseSize( metaBase ) );
            uint32 i = index;
            for( ; i < index + ( size + sgMetaTotalPad ); ++i )
                raw_buf[i] = bFree;
            index = i;
            metaBase = NextMetaBase( metaBase );
        }

        char* display_buf = new char[iCol];
        for( int i = 0; i < iCol; ++i ) {
            int j = static_cast< int >( ( i / float( iCol ) ) * mArenaSize );
            int k = static_cast< int >( ( ( i + 1 ) / float( iCol ) ) * mArenaSize );
            float delta = static_cast< float >( k-j );
            uint64 sum = 0;
            for( j; j<k; ++j) {
                sum += raw_buf[j];
            }
            std::cout << ( ( sum / delta ) > 0.5f ? details::sgBlockChar : ' ' );
        }
        std::cout << "] " << LocalFragmentation() * 100 <<"%\n";
    }
    else
    {
        // Print long version with all octets and initial sector status char.
        std::cout << "[";
        tMetaBase metaBase = mBlock;
        while( IsMetaBaseResident( metaBase ) ) {
            bool bFree = IsMetaBaseFree( metaBase );
            uint32 size = ( MetaBaseSize( metaBase ) );
            char disp   = bFree ? '-' : '+';
            char beg    = bFree ? '0' : '@';
            std::cout << beg;
            for( uint32 i = 0; i < size + sgMetaTotalPad - 1; ++i )
                std::cout << disp;
            metaBase = NextMetaBase( metaBase );
        }
        std::cout << "] " << LocalFragmentation() * 100 <<"%\n";
    }
}

uint8*
FShrinkableAllocArena::LowBlockAdress()
{
    return  mBlock;
}

uint8*
FShrinkableAllocArena::HighBlockAdress()
{
    return  mBlock + uint64( ArenaSize() );
}

const uint8*
FShrinkableAllocArena::LowBlockAdress() const
{
    return  mBlock;
}

const uint8*
FShrinkableAllocArena::HighBlockAdress() const
{
    return  mBlock + uint64( ArenaSize() );
}

uint64
FShrinkableAllocArena::BlockSize() const {
    return  mArenaSize;
}

bool
FShrinkableAllocArena::IsResident( tMetaBase iMetaBase ) {
    return  iMetaBase >= LowBlockAdress() && iMetaBase < HighBlockAdress();
}

FShrinkableAllocArena::FIterator
FShrinkableAllocArena::FindFirstMinAlloc( bool iUsed, byte_t iMinimumSizeBytes, const FIterator& iFrom )
{
    uint64 size = FMath::Min( uint64( MaxAllocSize() ), uint64( iMinimumSizeBytes ) );
    ULIS_ASSERT( size > 0, "Cannot alloc 0 bytes" );

    FIterator it = iFrom.IsValid() ? iFrom : Begin();
    ULIS_ASSERT( IsResident( it.MetaBase() ), "Non resident from param" );
    const FIterator end = End();
    while( it != end ) {
        if( it.IsUsed() == iUsed && it.NextSize() >= size )
            return  it;
        ++it;
    }
    return  FIterator::MakeNull();
}

//static
bool
FShrinkableAllocArena::Shrink( tClient iClient, byte_t iNewSize )
{
    ULIS_ASSERT( iClient, "Cannot shrink null client" );
    FIterator it( ( tClient )( iClient ) );
    ULIS_ASSERT( it.IsUsed(), "Don't shrink free parts !" );

    uint32 req = iNewSize;
    ULIS_ASSERT( req > 0, "New size cannot be 0" );

    uint32 chunkSize = it.NextSize();
    ULIS_ASSERT( req <= chunkSize, "New size cannot be greater than current size." );

    uint32 rem = chunkSize > req + sgMetaTotalPad ? chunkSize - ( req + sgMetaTotalPad ) : 0;

    if( rem == 0 )
        return false;

    it.SetNextSize( req );
    FIterator sec = it + 1;
    sec.CleanupMetaBase();
    sec.SetPrevSize( req );
    sec.SetNextSize( rem );
    FIterator::MergeFree( sec, sec + 1 );
    return  true;
}

void
FShrinkableAllocArena::Initialize() {
    uint64 initialFreeBufferSize = uint64( mArenaSize ) - uint64( sgMetaTotalPad ) * 2;
    FIterator begin = Begin();
    begin.CleanupMetaBase();
    begin.SetPrevSize( 0 );
    begin.SetNextSize( initialFreeBufferSize );
    FIterator end = End();
    end.FillMetaBase();
    end.SetPrevSize( initialFreeBufferSize );
    end.SetNextSize( 0 );
}

FShrinkableAllocArena::FIterator
FShrinkableAllocArena::Begin() {
    return  FIterator( ( tMetaBase )( mBlock ) );
}

FShrinkableAllocArena::FIterator
FShrinkableAllocArena::End() {
    return  FIterator( ( tMetaBase )( mBlock + uint64( mArenaSize ) - uint64( sgMetaTotalPad ) ) );
}

const FShrinkableAllocArena::FIterator
FShrinkableAllocArena::Begin() const {
    return  FIterator( ( tMetaBase )( mBlock ) );
}

const FShrinkableAllocArena::FIterator
FShrinkableAllocArena::End() const {
    return  FIterator( ( tMetaBase )( mBlock + uint64( mArenaSize ) - uint64( sgMetaTotalPad ) ) );
}

ULIS_NAMESPACE_END

