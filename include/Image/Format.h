// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Format.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Format utilities.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FFormatMetrics
/// @brief      The FFormatMetrics class provides a way to store cached information
///             extrapolated from the eFormat bitfield.
/// @details    The FFormatMetrics class is meant to be used when cached format info
///             is needed for organization of documents or in various
///             algorithms.
///
///             \sa IHasFormat
struct ULIS_API FFormatMetrics
{
    /*! Destructor. */
    ~FFormatMetrics();

    /*! Constructor from eFormat. */
    FFormatMetrics( eFormat iFMT );

    /*! Copy constructor. */
    FFormatMetrics( const FFormatMetrics& iOther );

    /*! Move constructor. */
    FFormatMetrics( FFormatMetrics&& iOther );

    /*! Copy assignment operator. */
    FFormatMetrics& operator=( const FFormatMetrics& iOther );

    /*! Cached Index Table */
    uint8*      IDT;
    /*! Cached Format */
    eFormat     FMT;
    /*! Cached Type */
    eType       TP;
    /*! Cached Color Model */
    eColorModel CM;
    /*! Cached Bytes Per Channel */
    uint8       BPC;
    /*! Cached Num Color Channels */
    uint8       NCC;
    /*! Cached Has Extra Alpha */
    uint8       HEA;
    /*! Cached ReverseSwapped Code */
    uint8       RSC;
    /*! Cached Samples Per Pixel */
    uint8       SPP;
    /*! Cached Bytes Per Pixel */
    uint8       BPP;
    /*! Cached Alpha Index */
    uint8       AID;
    /*! Cached Reversed */
    uint8       REV;
    /*! Cached Swapped */
    uint8       SWA;
    /*! Cached Premultiplied */
    uint8       PRE;
    /*! Cached Linear */
    uint8       LIN;
    /*! Cached Default Profile Code */
    uint8       PRO;
    /*! Cached Planar */
    uint8       PLA;
};

/////////////////////////////////////////////////////
/// @class      IHasFormat
/// @brief      The IHasFormat class provides an interface for accessing format
///             informations for image related classes that have a format.
/// @details    The IHasFormat class provides acces to a FFormatMetrics and interfaces
///             for easy access to format information.
///
///             \sa FFormatMetrics
class ULIS_API IHasFormat
{
protected:
    /*! Destructor. */
    ~IHasFormat()
    {}

    /*! Constructor from eFormat. */
    IHasFormat( eFormat iFormat )
        : mFormatMetrics( iFormat )
    {}

public:
    /*! Return the underlying FFormatMetrics struct. */
    ULIS_FORCEINLINE const FFormatMetrics& FormatMetrics() const {
        return  mFormatMetrics;
    }

    /*!
    Return the redirection table for models layout indexes.

    Models have intrinsic indices associated with the channels components, for
    example RGB models associate indexes with channels this way:
    ( ( R, 0 ), ( G, 1 ), ( B, 2 ) )

    For eFormat::Format_RGBA8, the index table might look like this: [0,1,2,3]

    For eFormat::Format_ABGR8, the index table might look like this: [3,2,1,0]

    For eFormat::Format_BGRA8, the index table might look like this: [2,1,0,3]
    */
    ULIS_FORCEINLINE const uint8* IndexTable() const {
        return  mFormatMetrics.IDT;
    }

    /*! Return the underlying eFormat value. */
    ULIS_FORCEINLINE eFormat Format() const {
        return  mFormatMetrics.FMT;
    }

    /*! Return the extrapolated eType type. */
    ULIS_FORCEINLINE eType Type() const {
        return  mFormatMetrics.TP;
    }

    /*! Return the extrapolated eColorModel model. */
    ULIS_FORCEINLINE eColorModel Model() const {
        return  mFormatMetrics.CM;
    }

    /*! Return the number of bytes per sample ( per channel ). For eFormat::Format_RGBA8, this value is 1. */
    ULIS_FORCEINLINE uint8 BytesPerSample() const {
        return  mFormatMetrics.BPC;
    }

    /*! Return the number of color channels, alpha is not part of the color channels. For RGB, this value is 3. */
    ULIS_FORCEINLINE uint8 NumColorChannels() const {
        return  mFormatMetrics.NCC;
    }

    /*! Return wether the format has an extra alpha channel. */
    ULIS_FORCEINLINE bool HasAlpha() const {
        return  mFormatMetrics.HEA;
    }

    /*! Return the number of samples or channels, alpha is taken into account. For eFormat::Format_RGBA8, this value is 4. */
    ULIS_FORCEINLINE uint8 SamplesPerPixel() const {
        return  mFormatMetrics.SPP;
    }

    /*! Return the number of bytes per pixel. For eFormat::Format_RGBA8, this value is 4. */
    ULIS_FORCEINLINE uint8 BytesPerPixel() const {
        return  mFormatMetrics.BPP;
    }

    /*! Return the redirected index of the alpha channel. */
    ULIS_FORCEINLINE uint8 AlphaIndex() const {
        return  mFormatMetrics.AID;
    }

    /*! Return wether the layout is reversed or not. */
    ULIS_FORCEINLINE bool Reversed() const {
        return  mFormatMetrics.REV;
    }

    /*! Return wether the layout is swapped or not. */
    ULIS_FORCEINLINE bool Swapped() const {
        return  mFormatMetrics.SWA;
    }

    /*! Return wether the format is premultiplied or not. */
    ULIS_FORCEINLINE bool Premultiplied() const {
        return  mFormatMetrics.PRE;
    }

    /*! Return wether the format is linear or not. */
    ULIS_FORCEINLINE bool Linear() const {
        return  mFormatMetrics.LIN;
    }

    /*! Return wether the format default profile identifier. */
    ULIS_FORCEINLINE uint8 DefaultProfileCode() const {
        return  mFormatMetrics.PRO;
    }

    /*!
    Return the redirected index for the given "natural" index.

    \sa IndexTable()
    */
    ULIS_FORCEINLINE uint8 RedirectedIndex( uint8 iIndex ) const {
        ULIS_ASSERT( iIndex >= 0 && iIndex < mFormatMetrics.SPP, "Bad Index" );
        return  mFormatMetrics.IDT[ iIndex ];
    }

protected:
    /*!
    Reinterpret the format by changing the value and extrapolating and caching
    the values again.

    This is not part of the public interface because there are strict
    constraints and circumstances under which and format reinterpretation is
    allowed. These circumstances are defined by the child classes.
    */
    ULIS_FORCEINLINE void ReinterpretFormat( eFormat iFormat ) {
        mFormatMetrics = FFormatMetrics( iFormat );
    }

private:
    FFormatMetrics mFormatMetrics; ///< The FormatMetrics.
};

ULIS_NAMESPACE_END

