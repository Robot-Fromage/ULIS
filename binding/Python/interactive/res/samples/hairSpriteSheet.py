from pyULIS4 import *
import math
import random

width = 512
height = 512
sizex = 8
sizey = 8
tilex = int( width / sizex )
tiley = int( height / sizey )

pool    = FThreadPool()
queue   = FCommandQueue( pool )
fmt     = Format_RGBA8
ctx     = FContext( queue, fmt, PerformanceIntent_SSE )
canvas  = FBlock( width, height, fmt )
elapsed = 0.0

print( FullLibraryInformationString() )
print( "Launch script hair." )

# Called once at the beginning of play.
def start():
    global fmt
    global ctx
    global canvas
    tile = FBlock( tilex, tiley, fmt )
    strip = FBlock( tilex, 1, fmt )
    line = FBlock( tilex, 1, fmt )
    ctx.Clear( canvas )
    ctx.Clear( tile )
    ctx.Fill( strip, FColor.Red )
    ctx.Finish()
    pi = 3.14159265359
    pi2 = pi / 2
    minNumWiggle = 8
    maxNumWiggle = 16
    rangeWiggle = int( tilex / 5 )
    for x in range( 0, sizex ):
        for y in range( 0, sizey ):
            currNumWiggle = random.randrange( minNumWiggle, maxNumWiggle )
            currRangeWiggle = random.randrange( 1, rangeWiggle )
            clearTile = FEvent()
            ctx.Clear( tile, event = clearTile )
            for i in range( 0, tiley ):
                clearLine = FEvent()
                resizeStrip = FEvent()
                ctx.Clear( line, event = clearLine )
                wiggle = i / 2 + ( 1 - i / tilex ) * ( math.sin( ( i / tiley ) * ( pi * currNumWiggle ) ) * currRangeWiggle )
                ctx.Resize( 
                      strip
                    , line
                    , dstRect = FRectF( 0, 0, tilex - i, 1 )
                    , resamplingMethod = Resampling_NearestNeighbour
                    , waitList = [ clearTile, clearLine ]
                    , event = resizeStrip
                )
                ctx.BlendAA(
                      line
                    , tile
                    , pos = FVec2F( wiggle, i )
                    , waitList = [ resizeStrip ]
                )
                ctx.Finish()
            ctx.Blend( tile, canvas, pos = FVec2I( x * tilex, y * tiley ) )
            ctx.Finish()

# Called every frame during play.
def update( delta ):
    global fmt
    global ctx
    global canvas
    global elapsed
    elapsed += delta

