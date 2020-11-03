import sys, os
sys.path.append( os.path.join( os.path.dirname(__file__), '..' ) )
import PyULIS3 as ul3

a = ul3.FBlock( 256, 256, ul3.Format_RGBA8 )
