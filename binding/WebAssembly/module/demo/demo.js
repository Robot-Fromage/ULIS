// wULIS4 Interactive WebAssembly Demo.
// Interaction beetween C++ compiled to WebAssembly and JS container.
// No documentation is available for the binding yet.
// You can use the C++ API reference instead:
// https://praxinos.coop/Documentation/ULIS/Developer/version/dev/html/
// Have fun,
// - Layl.
var elapsed = 0.0;
var pool = new wULIS4.FThreadPool( 1 );
var queue = new wULIS4.FCommandQueue( pool );
var fmt = wULIS4.eFormat.Format_RGBA8;
var ctx = new wULIS4.FContext( queue, fmt, wULIS4.ePerformanceIntent.PerformanceIntent_Max );
var w = 512;
var h = 512;
var block = new wULIS4.FBlock( w, h, fmt );

onStart = function() {
    stdout( wULIS4.FullLibraryInformationString() );
}

onUpdate = function( delta ) {
    elapsed += delta;
    var color = wULIS4.FColor.HSVA8( ( elapsed * 40 ) % 255, 255, 255, 255 );
    ctx.Fill( block, color, block.Rect() );
    ctx.Finish();
    refresh( block );
    color.delete();
}

