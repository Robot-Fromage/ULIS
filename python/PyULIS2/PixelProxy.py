import sys, os
sys.path.append( os.path.dirname(__file__) )
import _PyULIS2
from Types          import *
import AbstractPixel

class FPixelProxy( _PyULIS2._PyULIS2Type_PixelProxy, AbstractPixel._IAbstractPixel ):
    def __init__( self ): pass
