from pyULIS4 import *
import random
# Keep in mind the coordinate system
# starts at (0;0) in the bottom left
# corner, since we are working in an
# OpenGL context.
pool    = FThreadPool()
queue   = FCommandQueue( pool )
fmt     = Format_RGBA8
ctx     = FContext( queue, fmt, PerformanceIntent_SSE )
canvas  = FBlock( 800, 600, fmt )
particle = FBlock( 4, 4, fmt )
elapsed = 0.0
spawnPoint = FVec2F( canvas.Rect().Size() / 2 )
speed = 2
gravity = 0.1
maxAge = 40

class Particle:
    def __init__( self, age, pos, vel ):
        self.age = age
        self.pos = FVec2F( pos )
        self.vel = FVec2F( vel )
particles = []

# Called once at the beginning of play.
def start():
    global fmt
    global ctx
    global canvas
    global particle

    eventClear = FEvent()
    ctx.Clear( particle, event = eventClear )
    ctx.DrawCircleAndresAA(
          particle
        , particle.Rect().Size() / 2
        , int( particle.Rect().Size().x / 2 )
        , FColor.RGBA8( 40, 10, 5, 255 )
        , True
        , waitList = [ eventClear ]
    )
    ctx.Flush()
    ctx.Fence()

# Called every frame during play.
def update( delta ):
    global fmt
    global ctx
    global canvas
    global elapsed
    global gravity
    global spawnPoint
    global speed
    global maxAge
    elapsed += delta

    ctx.Fill( canvas, FColor.Black )
    ctx.Flush()

    for i in range( 0, random.randint( 1, 30 ) ):
        velx = random.uniform( 0, 2 ) - 1
        vely = random.uniform( 0, 2 ) - 1
        particles.append( Particle( random.randint( 20, maxAge ), spawnPoint, FVec2F( velx, vely ) * speed ) )

    for i in particles:
        i.pos += i.vel
        i.age -= 1
        i.vel.y += gravity
        ctx.Fence()
        ctx.BlendAA(
              particle
            , canvas
            , pos = i.pos
            , blendMode = Blend_Add
            , opacity = i.age / maxAge
            , policy = FSchedulePolicy.MonoScanlines
        )
        ctx.Flush()
        if i.age < 0:
            particles.remove( i )

    print( len( particles ) )
    ctx.Fence()

