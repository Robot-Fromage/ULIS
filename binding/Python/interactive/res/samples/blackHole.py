from pyULIS4 import *
import math
# Keep in mind the coordinate system
# starts at (0;0) in the bottom left
# corner, since we are working in an
# OpenGL context.
pool    = FThreadPool()
queue   = FCommandQueue( pool )
fmt     = Format_RGBA8
ctx     = FContext( queue, fmt )
canvas  = FBlock( 800, 600, fmt )
temp    = FBlock( 800, 600, fmt )
elapsed = 0.0

# Called once at the beginning of play.
def start():
    global fmt
    global ctx
    global canvas

# Called every frame during play.
def update( delta ):
    global fmt
    global ctx
    global canvas
    global elapsed
    elapsed += delta
    radius = ( ( math.sin( elapsed / 1 ) + 1 ) * 100 )
    eventFill = FEvent()
    eventClear = FEvent()
    eventCircle = FEvent()
    ctx.Fill( canvas, FColor.White, event = eventFill )
    ctx.Clear( temp, event = eventClear )
    ctx.Finish()
    ctx.DrawCircleBresenhamSP( temp, FVec2F( 400, 300 ), radius, FColor.Black, True, waitList = [ eventFill, eventClear ], event = eventCircle )
    ctx.Finish()
    ctx.Blend( temp, canvas, waitList = [ eventCircle ] )
    ctx.Finish()


