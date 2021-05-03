from pyULIS4 import *

#boilerplate
pool    = FThreadPool()
queue   = FCommandQueue( pool )
fmt     = Format_RGBA8
ctx     = FContext( queue, fmt )
fengine = FFontEngine()
font    = FFont( fengine, "Arial", "Black" )

#data
w       = 256
h       = 256
size    = FVec2F( w, h )
block   = FBlock( w, h, fmt )

#bake
grad = FGradient( fmt )
grad.AddColorStep( 0, FColor.Red )
grad.AddColorStep( 1, FColor.Blue )
s_grad = FSanitizedGradient( fmt, grad )

#process
eventGrad = FEvent()
eventText = FEvent()
ctx.RasterGradient( block, FVec2F(), size, s_grad, 0.015, event = eventGrad )
ctx.RasterTextAA( block, "pyULIS4", font, 16, FMat3F.MakeTranslationMatrix( 4, 20 ), FColor.White, waitList = [ eventGrad ], event = eventText )
ctx.SaveBlockToDisk( block, "out.png", waitList = [ eventText ] )
ctx.Finish()

