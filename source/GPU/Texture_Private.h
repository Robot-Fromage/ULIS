// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Texture_Private.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FTexture_Private class.
* @license      Please refer to LICENSE.md
*/
#ifdef ULIS_FEATURE_GPU_ENABLED

#pragma once
#include "Core/Core.h"
#include "Core/Callback.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Texture.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include <CL/cl.hpp>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FTexture_Private
/// @brief      The FTexture_Private class provides the actual implementation of
///             FTexture using the pimpl idiom.
class FTexture_Private final
{
public:
    /*! Destroy the texture and invoke the cleanup callback. */
    ~FTexture_Private();

    /*!
    Construct a texture with input size and format.

    The underlying bitmap data will be allocated with enough storage space as
    required by the size and format depth. The data is left uninitialized.

    \warning The \a iWidth and \a iHeight parameters should be greater than
    zero. A block doesn't own nor manage lifetime of its color-space.
    */
    FTexture_Private(
          uint16 iWidth
        , uint16 iHeight
        , const FOnInvalidTexture& iOnInvalid
        , const FOnCleanupData& iOnCleanup
        , FTexture& iParent
    );

    /*!
    explicitly disabled copy constructor.

    If you need to make a shallow copy with shared image data, use the explicit
    constructor from external data.

    If you need to make a deep copy of the image data, use the explicit Copy or
    XCopy functions that ULIS provides.
    */
    FTexture_Private( const FTexture_Private& ) = delete;

    /*!
    explicitly disabled copy assignment operator.

    If you need to make a shallow copy with shared image data, use the explicit
    constructor from external data.

    If you need to make a deep copy of the image data, use the explicit Copy or
    XCopy functions that ULIS provides.
    */
    FTexture_Private& operator=( const FTexture_Private& ) = delete;

    /*!
    Return the width of the block.

    \sa Height()
    \sa Area()
    \sa Rect()
    */
    uint16 Width() const;

    /*!
    Return the height of the block.

    \sa Width()
    \sa Area()
    \sa Rect()
    */
    uint16 Height() const;

    /*!
    Return the area of the block.

    This can also be interpreted as the full length of the underlying buffer.

    \sa Width()
    \sa Height()
    \sa Rect()
    */
    uint32 Area() const;

    /*!
    Return a rect representing the bounding box of the block.

    \sa Width()
    \sa Height()
    \sa Area()
    */
    FRectI Rect() const;

    /*!
    Return the numbers of bytes per scanline

    \sa BytesTotal()
    */
    uint32 BytesPerScanLine() const;

    /*!
    Return the total numbers of bytes in the block.

    \sa BytesPerScanLine()
    */
    uint64 BytesTotal() const;

    /*!
    Dirty the entire block and trigger the invalid callback if set.

    The optional \a iCallOnInvalid parameter can be set to false in order to
    conditionaly disable the callback trigger depending on your needs.

    In the case of many successive calls to dirty, it can be more efficient to
    just disable them first and call it only once later.
    */
    void Dirty( bool iCallOnInvalid = true ) const;

    /*!
    Dirty a part of the block according to the input rect and trigger the
    invalid callback if set.

    The optional \a iCallOnInvalid parameter can be set to false in order to
    conditionaly disable the callback trigger depending on your needs.

    In the case of many successive calls to dirty with different small
    geometries, it can be more efficient to just disable them first and call it
    only once later with the union of all invalid rects, or use the array
    version of the Dirty function.
    */
    void Dirty( const FRectI& iRect, bool iCallOnInvalid = true ) const;

    /*!
    Dirty parts of the block from the input array of rects and trigger the
    invalid callback if set.

    The optional \a iCallOnInvalid parameter can be set to false in order to
    conditionaly disable the callback trigger depending on your needs.
    */
    void Dirty( const FRectI* iRectList, const uint32 iNumRects = 1, bool iCallOnInvalid = true ) const;

    /*!
    Set a new invalid callback that will be called on dirty.

    \sa OnCleanup()
    */
    void OnInvalid( const FOnInvalidTexture& iOnInvalid );

    /*!
    Set a new cleanup callback that will be called on destruction.

    \sa OnInvalid()
    */
    void OnCleanup( const FOnCleanupData& iOnCleanup );

private:
    uint16 mWidth; ///< The width of the block.
    uint16 mHeight; ///< Height of the block.
    uint32 mBytesPerScanline; ///< Cached number of bytes per scanline.
    uint64 mBytesTotal; ///< Cached number of bytes for the whole buffer.
    FOnInvalidTexture mOnInvalid; ///< The callback for when the block is destroyed.
    FOnCleanupData mOnCleanup; ///< The callback for when the block is dirty.
    FTexture& mParent; ///< The parent interface, that displays the Format and Colorspace APIs.
    cl::Buffer mBitmap; ///< Assuming OpenCL 1.2 buffer mapped in device memory.
};

ULIS_NAMESPACE_END

#endif // ULIS_FEATURE_GPU_ENABLED

