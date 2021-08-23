import sys, os
sys.path.append( os.path.dirname(__file__) )
import _PyULIS2
from Types          import *
import AbstractPixel

class FPixelValue( AbstractPixel._IAbstractPixel, _PyULIS2._PyULIS2Type_PixelValue ):
    def __init__( self ):
        super( FPixelValue, self ).__init__()