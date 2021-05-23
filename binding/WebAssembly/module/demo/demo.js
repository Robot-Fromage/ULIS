// wULIS4 Interactive WebAssembly Demo.
// Interaction beetween C++ compiled to WebAssembly and JS container.
// No documentation is available for the binding yet.
// You can use the C++ API reference instead:
// https://praxinos.coop/Documentation/ULIS/Developer/version/dev/html/
// Have fun,
// - Layl.
var elapsed = 0.0;

onStart = function() {
    stdout( wULIS4.FullLibraryInformationString() );
    stdout( wULIS4.FThreadPool.MaxWorkers() );
    var pool = new wULIS4.FThreadPool( 1 );
    pool.delete();
}

onUpdate = function( delta ) {
    elapsed += delta;
    var block = new wULIS4.FBlock(
          512
        , 512
        , wULIS4.eFormat.Format_RGBA8
        , null
        , new wULIS4.FOnInvalidBlock()
        , wULIS4.FOnCleanupData.OnCleanupFreeMemory()
    );

    var color = wULIS4.FColor.HSVA8( ( elapsed * 80 ) % 255, 255, 255, 255 );
    for( let y = 0; y < block.Height(); y++ ) {
        for( let x = 0; x < block.Width(); x++ ) {
            block.SetPixel( x, y, color );
        }
    }

    refresh( block );

    color.delete();
    block.delete();
}

