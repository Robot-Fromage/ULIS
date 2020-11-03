import sys, os
sys.path.append( os.path.dirname(__file__) )
import _PyULIS2
from Types import *

class FBlock:
    def __init__( self, width, height, format ): self._data = _PyULIS2._PyULIS2Type_Block( width, height, format )
    def Width               (self): return  self._data.Width()
    def Height              (self): return  self._data.Height()
    def Format              (self): return  self._data.Format()
    def BytesPerSample      (self): return  self._data.BytesPerSample()
    def BytesPerPixel       (self): return  self._data.BytesPerPixel()
    def BytesPerScanLine    (self): return  self._data.BytesPerScanLine()
    def BytesTotal          (self): return  self._data.BytesTotal()
    def Model               (self): return  self._data.Model()
    def Type                (self): return  self._data.Type()
    def HasAlpha            (self): return  self._data.HasAlpha()
    def Swapped             (self): return  self._data.Swapped()
    def Reversed            (self): return  self._data.Reversed()
    def SamplesPerPixel     (self): return  self._data.SamplesPerPixel()
    def NumColorChannels    (self): return  self._data.NumColorChannels()
    def AlphaIndex          (self): return  self._data.AlphaIndex()
    def CRC32               (self): return  self._data.CRC32()
    def MD5                 (self): return  self._data.MD5()
    def UUID                (self): return  self._data.UUID()
    def TypeStr             (self): return  kwType[ self.Type() ]
    def ModelStr            (self): return  kwColorModel[ self.Model() ]

    def LayoutIndex( self ):
        spp = self.SamplesPerPixel()
        msp = spp - 1
        idt = [0] * spp
        cod = self.Swapped() << 1 | self.Reversed()
        if cod == 1:
            for i in range( 0, spp ):
                idt[i] = msp - i
        elif cod == 2:
            for i in range( 0, spp ):
                if ( i + 1 ) > msp:
                    idt[i] = 0
                else:
                    idt[i] = i + 1
        elif cod == 3:
            for i in range( 0, spp ):
                if ( msp - i ) - 1 < 0:
                    idt[i] = msp
                else:
                    idt[i] = msp - i - 1
        else:
            for i in range( 0, spp ):
                idt[i] = i
        return  idt

    def LayoutStr( self ):
        spp = self.SamplesPerPixel()
        idt = self.LayoutIndex()
        modelstr = kwColorModelA[ self.Model() ]
        str = ""
        for i in range( 0, spp ):
            str = str + modelstr[i]
        return  str

    def TypeSuffixStr( self ):
        return  kwTypeA[ self.Type() ]

    def FormatStr( self ):
        str = "Format_" + self.LayoutStr() + self.TypeSuffixStr()
        return  str

    def PrintInfo( self ):
        print( "Width               : ", self.Width() )
        print( "Height              : ", self.Height() )
        print( "Format              : ", self.Format() )
        print( "BytesPerSample      : ", self.BytesPerSample() )
        print( "BytesPerPixel       : ", self.BytesPerPixel() )
        print( "BytesPerScanLine    : ", self.BytesPerScanLine() )
        print( "BytesTotal          : ", self.BytesTotal() )
        print( "Model               : ", self.Model() )
        print( "Type                : ", self.Type() )
        print( "HasAlpha            : ", self.HasAlpha() )
        print( "Swapped             : ", self.Swapped() )
        print( "Reversed            : ", self.Reversed() )
        print( "SamplesPerPixel     : ", self.SamplesPerPixel() )
        print( "NumColorChannels    : ", self.NumColorChannels() )
        print( "AlphaIndex          : ", self.AlphaIndex() )
        print( "CRC32               : ", self.CRC32() )
        print( "MD5                 : ", self.MD5() )
        print( "UUID                : ", self.UUID() )
        print( "TypeStr             : ", self.TypeStr() )
        print( "ModelStr            : ", self.ModelStr() )
        print( "LayoutIndex         : ", self.LayoutIndex() )
        print( "LayoutStr           : ", self.LayoutStr() )
        print( "TypeSuffixStr       : ", self.TypeSuffixStr() )
        print( "FormatStr           : ", self.FormatStr() )

    def PixelProxy( self, x, y ):
        return  self._data.PixelProxy( x, y )


