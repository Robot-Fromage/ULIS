import sys, os
sys.path.append( os.path.join( os.path.dirname(__file__), '..' ) )
import PyULIS2 as ul2

block = ul2.FBlock( 256, 256, ul2.Format_RGBA8 )
block.PrintInfo()

