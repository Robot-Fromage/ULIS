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
elapsed = 0.0
fade    = FBlock( 800, 600, fmt )
round   = FBlock( 32, 32, fmt )

# Called once at the beginning of play.
def start():
    global fmt
    global ctx
    global canvas
    global fade
    global round
    ctx.Fill( canvas, FColor.RGBA8( 30, 30, 30, 255 )
    ctx.Clear( fade )
    ctx.Clear( round )
    ctx.Finish()
    ctx.XCreateTestBlock( round )
    ctx.Finish()
    rot = FMat3F.MakeRotationMatrix( 3.14 / 2 + 3.14 / 4 )
    trans = FMat3F.MakeTranslationMatrix( 400, 300 )
    scale = FMat3F.MakeScaleMatrix( -5.5, 5.5 )
    matrix = trans * rot * scale
    ctx.TransformAffine( round, fade, mat = matrix )
    ctx.Finish()
    ctx.Blend( fade, canvas )

# Called every frame during play.
def update( delta ):
    #global fmt
    #global ctx
    #global canvas
    #global elapsed
    #global fade
    #global round
    #elapsed += delta
    #ctx.Blend( fade, canvas, opacity = 0.25 )
    #ctx.Finish()
    #center = FVec2F( canvas.Rect().Size() / 2 )
    #radius = int( center.y ) - 50
    #shift = FVec2F( round.Rect().Size() / 2 )
    #disp = FVec2F( math.cos( elapsed ), math.sin( elapsed ) )
    #ctx.BlendAA( round, canvas, pos = center + disp * radius - shift )
    ctx.Finish()


