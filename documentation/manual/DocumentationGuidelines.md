# Documentation Guidelines

Document classes and structs that are part of the public API ( ULIS_API ) like so:

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


Document every public methods of classes and structs that are part of the public API ( they can be multiline ):

    /*!
    Construct a block from an existing external buffer with input size and
    format.

    The input buffer data is used and no allocation is done here, if coherent
    visual data was present in the buffer it will remain as is. Make sure to
    chose the appropriate size and format, given the buffer length. Bad values
    could result in misinterpreting the visual information or accessing the
    buffer out of bounds and crash or corrupt memory.

    \warning The block will not delete the external input data by default,
    unless you provide and appropriate cleanup callback to handle that. The
    data can be shared with other representations but it must remain valid at
    least as long as the block lifetime.

    \warning The \a iWidth and \a iHeight parameters should be greater than
    zero. A block doesn't own nor manage lifetime of its color-space.
    */
    FBlock(
          uint8* iData
        , uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , const FColorSpace* iColorSpace = nullptr
        , const FOnInvalidBlock& iOnInvalid = FOnInvalidBlock()
        , const FOnCleanupData& iOnCleanup = FOnCleanupData()
    );
    

Document every fields of the public APIs. In short, everything in headers that are visible by consumers of the library, so all files under the include directory.  

    /*! The x component of the 2D vector. */
    T x;
    /*! The y component of the 2D vector. */
    T y;


