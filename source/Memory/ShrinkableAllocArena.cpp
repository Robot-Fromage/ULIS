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
    return  *this;
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
FShrinkableAllocArena::FIterator::operator+( int iValue ) const {
    FIterator it( *this );
    for( int i = 0; i < iValue; ++i )
        ++it;
    return it;
}

FShrinkableAllocArena::FIterator
FShrinkableAllocArena::FIterator::operator-( int iValue ) const {
    FIterator it( *this );
    for( int i = 0; i < iValue; ++i )
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
    return  mMetaBase != nullptr;
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
    memset( mMetaBase, 0, sizeof( tClient ) );
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
    return  IsValid() ? *(tClient*)( mMetaBase ) = new tAlloc( Allocation() ) : nullptr;
}

//static
void
FShrinkableAllocArena::FIterator::MergeFree( const FIterator& iA, const FIterator& iB ) {
    ULIS_ASSERT( ( iA + 1 ) == iB, "Bad" );

    if( iA.IsUsed() || iB.IsUsed() )
        return;

    FIterator n0 = iA;
    FIterator n1 = iB + 1;
    uint32 mergeSize = n0.NextSize() + sgMetaTotalPad + n1.PrevSize();
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
    uint32 req = FMath::Min( uint32( MaxAllocSize() ), uint32( iSize ) );
    FIterator it = FindFirstMinAlloc( false, req );

    if( !it.IsValid() )
        return  nullptr;

    it.AllocClient();
    uint32 chunkSize = it.NextSize();
    uint32 rem = chunkSize > req + sgMetaTotalPad ? chunkSize - ( req + sgMetaTotalPad ) : 0;

    if( rem == 0 )
        return it.Client();

    it.SetNextSize( req );
    FIterator sec = it + 1;
    sec.CleanupMetaBase();
    sec.SetPrevSize( req );
    sec.SetNextSize( rem );
    FIterator tir = sec + 1;
    tir.SetPrevSize( rem );
    FIterator::MergeFree( sec, tir );

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
    if( !it.IsBegin() )
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
    uint64 runFree, maxFree, runUsed, maxUsed, totalFree, totalUsed;
    runFree = maxFree = runUsed = maxUsed = totalFree = totalUsed = 0;
    const FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        uint64 size = it.NextSize();
        if( it.IsFree() ) {
            runFree += size;
            totalFree += size;
            runUsed = 0;
        } else {
            runUsed += size;
            totalUsed += size;
            runFree = 0;
        }
        maxFree = FMath::Max( runFree, maxFree );
        maxUsed = FMath::Max( runUsed, maxUsed );
        ++it;
    }

    if( totalFree == 0 || totalUsed == 0 )
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
    FIterator it = Begin();
    const FIterator end = End();
    while( it != end ) {
        if( it.IsFree() ) {
            uint32 backup_prev = it.PrevSize();
            uint32 backup_next = it.NextSize();
            FIterator src = FindFirstMinAlloc( true, 1, it );
            if( !src.IsValid() )
                break;
            tMetaBase base = src.MetaBase();
            uint64 run = 0;
            while( src.IsValid() && src.IsUsed() && src != end ) {
                run += src.NextSize() + sgMetaTotalPad;
                ++src;
            }
            uint32 bd = ( src - 1 ).NextSize();
            memmove( it.MetaBase(), base, run );
            it.SetPrevSize( backup_prev );
            FIterator last( it.MetaBase() + run );
            last.CleanupMetaBase();
            last.SetPrevSize( bd );
            last.SetNextSize( backup_next );
            src.SetPrevSize( backup_next );
            FIterator::MergeFree( last, src );
            it = last;
        } else {
            ++it;
        }
    }
}

void
FShrinkableAllocArena::DebugPrint( int iType, int iCol ) const
{
    if( iType == 0 )
    {
        // Print short version, scaled down with rough estimate of sector status.
        std::cout << "[";
        uint8* raw_buf = new uint8[mArenaSize - sgMetaTotalPad ];
        int index = 0;
        FIterator it = Begin();
        const FIterator end = End();
        while( it != end ) {
            const bool bFree = it.IsFree();
            const uint32 size = it.NextSize();
            uint32 i = index;
            for( ; i < index + ( size + sgMetaTotalPad ); ++i )
                raw_buf[i] = !bFree;
            index = i;
            ++it;
        }

        char* display_buf = new char[iCol];
        for( int i = 0; i < iCol; ++i ) {
            int j = static_cast< int >( ( i / float( iCol ) ) * ( mArenaSize - sgMetaTotalPad ) );
            int k = static_cast< int >( ( ( i + 1 ) / float( iCol ) ) * ( mArenaSize - sgMetaTotalPad ) );
            float delta = static_cast< float >( k-j );
            uint64 sum = 0;
            for( j; j<k; ++j) {
                sum += raw_buf[j];
            }
            std::cout << ( ( sum / delta ) > 0.5f ? sgBlockChar : ' ' );
        }
        std::cout << "] " << LocalFragmentation() * 100 <<"%\n";
    }
    else if( iType == 1 )
    {
        // Print long version with all octets and initial sector status char.
        std::cout << "[";
        FIterator it = Begin();
        const FIterator end = End();
        while( it != end ) {
            const bool bFree = it.IsFree();
            const uint32 size = it.NextSize();
            char disp   = bFree ? '-' : '+';
            char beg    = bFree ? '0' : '@';
            std::cout << beg;
            for( uint32 i = 0; i < size + sgMetaTotalPad - 1; ++i )
                std::cout << disp;
            ++it;
        }
        std::cout << "] " << LocalFragmentation() * 100 <<"%\n";
    }
    else
    {
        // Print individual allocs states
        std::cout << "[\n";
        FIterator it = Begin();
        const FIterator end = End();
        while( it != end ) {
            char free = it.IsFree() ? 'Y' : 'N';
            std::cout << "[ Free: " << free << " Prev: " << it.PrevSize() << " Next: " << it.NextSize() << " Adress: " << (intptr_t)(it.MetaBase()) << " ] " << std::endl;
            ++it;
        }
        char free = it.IsFree() ? 'Y' : 'N';
        std::cout << "[ Free: " << free << " Prev: " << it.PrevSize() << " Next: " << it.NextSize() << " Adress: " << (intptr_t)(it.MetaBase()) << " ] " << std::endl;
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
    FIterator tir = sec + 1;
    tir.SetPrevSize( rem );
    FIterator::MergeFree( sec, tir );

    return  true;
}

void
FShrinkableAllocArena::Initialize() {
    uint32 initialFreeBufferSize = uint32( mArenaSize ) - uint32( sgMetaTotalPad ) * 2;
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

