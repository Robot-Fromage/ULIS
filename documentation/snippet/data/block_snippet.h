//! [FBlock on stack]
    { // Begin local scope
        FBlock image( 64, 64, eFormat::Format_RGBA8 );
        // Work with your image.
    } // End local scope, image is destroyed and so is the underlying data.
//! [FBlock on stack]

//! [FBlock on heap]
    FBlock* image = new FBlock( 64, 64, eFormat::Format_RGBA8 );
    // Work with your image.
    delete  image;
//! [FBlock on heap]

//! [FBlock X Version]
    FBlock* image = FBlock::XMake( 64, 64, eFormat::Format_RGBA8 );
    // Work with your image.
    FBlock::XDelete( image );
//! [FBlock X Version]
