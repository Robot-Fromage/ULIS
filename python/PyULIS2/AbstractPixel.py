import sys, os
sys.path.append( os.path.dirname(__file__) )
import _PyULIS2
from Types import *

class _IAbstractPixel( _PyULIS2._PyULIS2Type_AbstractPixel ):
    def __init__            ( self ): pass
    def LayoutIndex( self ):
        spp = self.NumSamples()
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
        spp = self.NumSamples()
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

    def MSP( self ):
        return  self.NumSamples() - 1

    def Alpha8( self ):
        if self.HasAlpha():
            return  self.GetValue8( self.MSP() )
        else:
            return  255

    def Alpha16( self ):
        if self.HasAlpha():
            return  self.GetValue16( self.MSP() )
        else:
            return  65535

    def Alpha32( self ):
        if self.HasAlpha():
            return  self.GetValue32( self.MSP() )
        else:
            return  4294967295

    def AlphaF( self ):
        if self.HasAlpha():
            return  self.GetValueF( self.MSP() )
        else:
            return  1.0

    def AlphaD( self ):
        if self.HasAlpha():
            return  self.GetValueF( self.MSP() )
        else:
            return  1.0

    def SetAlpha8( self, i ):
        if self.HasAlpha():
            return  self.SetValue8( self.MSP(), int(i) )

    def Alpha16( self, i ):
        if self.HasAlpha():
            return  self.SetValue16( self.MSP(), int(i) )

    def Alpha32( self, i ):
        if self.HasAlpha():
            return  self.GetValue32( self.MSP(), int(i) )

    def AlphaF( self, i ):
        if self.HasAlpha():
            return  self.GetValueF( self.MSP(), float(i) )

    def AlphaD( self, i ):
        if self.HasAlpha():
            return  self.GetValueF( self.MSP(), float(i) )

    def A8          ( self ):   return  self.Alpha8()
    def Grey8       ( self ):   return  self.GetValue8( 0 )
    def Red8        ( self ):   return  self.GetValue8( 0 )
    def Green8      ( self ):   return  self.GetValue8( 1 )
    def Blue8       ( self ):   return  self.GetValue8( 2 )
    def R8          ( self ):   return  self.GetValue8( 0 )
    def G8          ( self ):   return  self.GetValue8( 1 )
    def B8          ( self ):   return  self.GetValue8( 2 )
    def Hue8        ( self ):   return  self.GetValue8( 0 )
    def Saturation8 ( self ):   return  self.GetValue8( 1 )
    def Lightness8  ( self ):   return  self.GetValue8( 2 )
    def Value8      ( self ):   return  self.GetValue8( 2 )
    def Cyan8       ( self ):   return  self.GetValue8( 0 )
    def Magenta8    ( self ):   return  self.GetValue8( 1 )
    def Yellow8     ( self ):   return  self.GetValue8( 2 )
    def Key8        ( self ):   return  self.GetValue8( 3 )
    def Luma8       ( self ):   return  self.GetValue8( 0 )
    def U8          ( self ):   return  self.GetValue8( 1 )
    def V8          ( self ):   return  self.GetValue8( 2 )
    def K8          ( self ):   return  self.GetValue8( 3 )
    def X8          ( self ):   return  self.GetValue8( 0 )
    def Y8          ( self ):   return  self.GetValue8( 1 )
    def Z8          ( self ):   return  self.GetValue8( 2 )
    def L8          ( self ):   return  self.GetValue8( 0 )
    def a8          ( self ):   return  self.GetValue8( 1 )
    def b8          ( self ):   return  self.GetValue8( 2 )
    def x8          ( self ):   return  self.GetValue8( 1 )
    def y8          ( self ):   return  self.GetValue8( 2 )
    def A16         ( self ):   return  self.Alpha16()
    def Grey16      ( self ):   return  self.GetValue16( 0 )
    def Red16       ( self ):   return  self.GetValue16( 0 )
    def Green16     ( self ):   return  self.GetValue16( 1 )
    def Blue16      ( self ):   return  self.GetValue16( 2 )
    def R16         ( self ):   return  self.GetValue16( 0 )
    def G16         ( self ):   return  self.GetValue16( 1 )
    def B16         ( self ):   return  self.GetValue16( 2 )
    def Hue16       ( self ):   return  self.GetValue16( 0 )
    def Saturation16( self ):   return  self.GetValue16( 1 )
    def Lightness16 ( self ):   return  self.GetValue16( 2 )
    def Value16     ( self ):   return  self.GetValue16( 2 )
    def Cyan16      ( self ):   return  self.GetValue16( 0 )
    def Magenta16   ( self ):   return  self.GetValue16( 1 )
    def Yellow16    ( self ):   return  self.GetValue16( 2 )
    def Key16       ( self ):   return  self.GetValue16( 3 )
    def Luma16      ( self ):   return  self.GetValue16( 0 )
    def U16         ( self ):   return  self.GetValue16( 1 )
    def V16         ( self ):   return  self.GetValue16( 2 )
    def K16         ( self ):   return  self.GetValue16( 3 )
    def X16         ( self ):   return  self.GetValue16( 0 )
    def Y16         ( self ):   return  self.GetValue16( 1 )
    def Z16         ( self ):   return  self.GetValue16( 2 )
    def L16         ( self ):   return  self.GetValue16( 0 )
    def a16         ( self ):   return  self.GetValue16( 1 )
    def b16         ( self ):   return  self.GetValue16( 2 )
    def x16         ( self ):   return  self.GetValue16( 1 )
    def y16         ( self ):   return  self.GetValue16( 2 )
    def A32         ( self ):   return  self.Alpha32()
    def Grey32      ( self ):   return  self.GetValue32( 0 )
    def Red32       ( self ):   return  self.GetValue32( 0 )
    def Green32     ( self ):   return  self.GetValue32( 1 )
    def Blue32      ( self ):   return  self.GetValue32( 2 )
    def R32         ( self ):   return  self.GetValue32( 0 )
    def G32         ( self ):   return  self.GetValue32( 1 )
    def B32         ( self ):   return  self.GetValue32( 2 )
    def Hue32       ( self ):   return  self.GetValue32( 0 )
    def Saturation32( self ):   return  self.GetValue32( 1 )
    def Lightness32 ( self ):   return  self.GetValue32( 2 )
    def Value32     ( self ):   return  self.GetValue32( 2 )
    def Cyan32      ( self ):   return  self.GetValue32( 0 )
    def Magenta32   ( self ):   return  self.GetValue32( 1 )
    def Yellow32    ( self ):   return  self.GetValue32( 2 )
    def Key32       ( self ):   return  self.GetValue32( 3 )
    def Luma32      ( self ):   return  self.GetValue32( 0 )
    def U32         ( self ):   return  self.GetValue32( 1 )
    def V32         ( self ):   return  self.GetValue32( 2 )
    def K32         ( self ):   return  self.GetValue32( 3 )
    def X32         ( self ):   return  self.GetValue32( 0 )
    def Y32         ( self ):   return  self.GetValue32( 1 )
    def Z32         ( self ):   return  self.GetValue32( 2 )
    def L32         ( self ):   return  self.GetValue32( 0 )
    def a32         ( self ):   return  self.GetValue32( 1 )
    def b32         ( self ):   return  self.GetValue32( 2 )
    def x32         ( self ):   return  self.GetValue32( 1 )
    def y32         ( self ):   return  self.GetValue32( 2 )
    def AF          ( self ):   return  self.AlphaF()
    def GreyF       ( self ):   return  self.GetValueF( 0 )
    def RedF        ( self ):   return  self.GetValueF( 0 )
    def GreenF      ( self ):   return  self.GetValueF( 1 )
    def BlueF       ( self ):   return  self.GetValueF( 2 )
    def RF          ( self ):   return  self.GetValueF( 0 )
    def GF          ( self ):   return  self.GetValueF( 1 )
    def BF          ( self ):   return  self.GetValueF( 2 )
    def HueF        ( self ):   return  self.GetValueF( 0 )
    def SaturationF ( self ):   return  self.GetValueF( 1 )
    def LightnessF  ( self ):   return  self.GetValueF( 2 )
    def ValueF      ( self ):   return  self.GetValueF( 2 )
    def CyanF       ( self ):   return  self.GetValueF( 0 )
    def MagentaF    ( self ):   return  self.GetValueF( 1 )
    def YellowF     ( self ):   return  self.GetValueF( 2 )
    def KeyF        ( self ):   return  self.GetValueF( 3 )
    def LumaF       ( self ):   return  self.GetValueF( 0 )
    def UF          ( self ):   return  self.GetValueF( 1 )
    def VF          ( self ):   return  self.GetValueF( 2 )
    def KF          ( self ):   return  self.GetValueF( 3 )
    def XF          ( self ):   return  self.GetValueF( 0 )
    def YF          ( self ):   return  self.GetValueF( 1 )
    def ZF          ( self ):   return  self.GetValueF( 2 )
    def LF          ( self ):   return  self.GetValueF( 0 )
    def aF          ( self ):   return  self.GetValueF( 1 )
    def bF          ( self ):   return  self.GetValueF( 2 )
    def xF          ( self ):   return  self.GetValueF( 1 )
    def yF          ( self ):   return  self.GetValueF( 2 )
    def AD          ( self ):   return  self.AlphaD()
    def GreyD       ( self ):   return  self.GetValueD( 0 )
    def RedD        ( self ):   return  self.GetValueD( 0 )
    def GreenD      ( self ):   return  self.GetValueD( 1 )
    def BlueD       ( self ):   return  self.GetValueD( 2 )
    def RD          ( self ):   return  self.GetValueD( 0 )
    def GD          ( self ):   return  self.GetValueD( 1 )
    def BD          ( self ):   return  self.GetValueD( 2 )
    def HueD        ( self ):   return  self.GetValueD( 0 )
    def SaturationD ( self ):   return  self.GetValueD( 1 )
    def LightnessD  ( self ):   return  self.GetValueD( 2 )
    def ValueD      ( self ):   return  self.GetValueD( 2 )
    def CyanD       ( self ):   return  self.GetValueD( 0 )
    def MagentaD    ( self ):   return  self.GetValueD( 1 )
    def YellowD     ( self ):   return  self.GetValueD( 2 )
    def KeyD        ( self ):   return  self.GetValueD( 3 )
    def LumaD       ( self ):   return  self.GetValueD( 0 )
    def UD          ( self ):   return  self.GetValueD( 1 )
    def VD          ( self ):   return  self.GetValueD( 2 )
    def KD          ( self ):   return  self.GetValueD( 3 )
    def XD          ( self ):   return  self.GetValueD( 0 )
    def YD          ( self ):   return  self.GetValueD( 1 )
    def ZD          ( self ):   return  self.GetValueD( 2 )
    def LD          ( self ):   return  self.GetValueD( 0 )
    def aD          ( self ):   return  self.GetValueD( 1 )
    def bD          ( self ):   return  self.GetValueD( 2 )
    def xD          ( self ):   return  self.GetValueD( 1 )
    def yD          ( self ):   return  self.GetValueD( 2 )

    def SetA8          ( self, i ):     self.SetAlpha8( i )
    def SetGrey8       ( self, i ):     self.SetValue8( 0, i )
    def SetRed8        ( self, i ):     self.SetValue8( 0, i )
    def SetGreen8      ( self, i ):     self.SetValue8( 1, i )
    def SetBlue8       ( self, i ):     self.SetValue8( 2, i )
    def SetR8          ( self, i ):     self.SetValue8( 0, i )
    def SetG8          ( self, i ):     self.SetValue8( 1, i )
    def SetB8          ( self, i ):     self.SetValue8( 2, i )
    def SetHue8        ( self, i ):     self.SetValue8( 0, i )
    def SetSaturation8 ( self, i ):     self.SetValue8( 1, i )
    def SetLightness8  ( self, i ):     self.SetValue8( 2, i )
    def SetValue8      ( self, i ):     self.SetValue8( 2, i )
    def SetCyan8       ( self, i ):     self.SetValue8( 0, i )
    def SetMagenta8    ( self, i ):     self.SetValue8( 1, i )
    def SetYellow8     ( self, i ):     self.SetValue8( 2, i )
    def SetKey8        ( self, i ):     self.SetValue8( 3, i )
    def SetLuma8       ( self, i ):     self.SetValue8( 0, i )
    def SetU8          ( self, i ):     self.SetValue8( 1, i )
    def SetV8          ( self, i ):     self.SetValue8( 2, i )
    def SetK8          ( self, i ):     self.SetValue8( 3, i )
    def SetX8          ( self, i ):     self.SetValue8( 0, i )
    def SetY8          ( self, i ):     self.SetValue8( 1, i )
    def SetZ8          ( self, i ):     self.SetValue8( 2, i )
    def SetL8          ( self, i ):     self.SetValue8( 0, i )
    def Seta8          ( self, i ):     self.SetValue8( 1, i )
    def Setb8          ( self, i ):     self.SetValue8( 2, i )
    def Setx8          ( self, i ):     self.SetValue8( 1, i )
    def Sety8          ( self, i ):     self.SetValue8( 2, i )
    def SetA16         ( self, i ):     self.SetAlpha16( i )
    def SetGrey16      ( self, i ):     self.SetValue16( 0, i )
    def SetRed16       ( self, i ):     self.SetValue16( 0, i )
    def SetGreen16     ( self, i ):     self.SetValue16( 1, i )
    def SetBlue16      ( self, i ):     self.SetValue16( 2, i )
    def SetR16         ( self, i ):     self.SetValue16( 0, i )
    def SetG16         ( self, i ):     self.SetValue16( 1, i )
    def SetB16         ( self, i ):     self.SetValue16( 2, i )
    def SetHue16       ( self, i ):     self.SetValue16( 0, i )
    def SetSaturation16( self, i ):     self.SetValue16( 1, i )
    def SetLightness16 ( self, i ):     self.SetValue16( 2, i )
    def SetValue16     ( self, i ):     self.SetValue16( 2, i )
    def SetCyan16      ( self, i ):     self.SetValue16( 0, i )
    def SetMagenta16   ( self, i ):     self.SetValue16( 1, i )
    def SetYellow16    ( self, i ):     self.SetValue16( 2, i )
    def SetKey16       ( self, i ):     self.SetValue16( 3, i )
    def SetLuma16      ( self, i ):     self.SetValue16( 0, i )
    def SetU16         ( self, i ):     self.SetValue16( 1, i )
    def SetV16         ( self, i ):     self.SetValue16( 2, i )
    def SetK16         ( self, i ):     self.SetValue16( 3, i )
    def SetX16         ( self, i ):     self.SetValue16( 0, i )
    def SetY16         ( self, i ):     self.SetValue16( 1, i )
    def SetZ16         ( self, i ):     self.SetValue16( 2, i )
    def SetL16         ( self, i ):     self.SetValue16( 0, i )
    def Seta16         ( self, i ):     self.SetValue16( 1, i )
    def Setb16         ( self, i ):     self.SetValue16( 2, i )
    def Setx16         ( self, i ):     self.SetValue16( 1, i )
    def Sety16         ( self, i ):     self.SetValue16( 2, i )
    def SetA32         ( self, i ):     self.SetAlpha32( i )
    def SetGrey32      ( self, i ):     self.SetValue32( 0, i )
    def SetRed32       ( self, i ):     self.SetValue32( 0, i )
    def SetGreen32     ( self, i ):     self.SetValue32( 1, i )
    def SetBlue32      ( self, i ):     self.SetValue32( 2, i )
    def SetR32         ( self, i ):     self.SetValue32( 0, i )
    def SetG32         ( self, i ):     self.SetValue32( 1, i )
    def SetB32         ( self, i ):     self.SetValue32( 2, i )
    def SetHue32       ( self, i ):     self.SetValue32( 0, i )
    def SetSaturation32( self, i ):     self.SetValue32( 1, i )
    def SetLightness32 ( self, i ):     self.SetValue32( 2, i )
    def SetValue32     ( self, i ):     self.SetValue32( 2, i )
    def SetCyan32      ( self, i ):     self.SetValue32( 0, i )
    def SetMagenta32   ( self, i ):     self.SetValue32( 1, i )
    def SetYellow32    ( self, i ):     self.SetValue32( 2, i )
    def SetKey32       ( self, i ):     self.SetValue32( 3, i )
    def SetLuma32      ( self, i ):     self.SetValue32( 0, i )
    def SetU32         ( self, i ):     self.SetValue32( 1, i )
    def SetV32         ( self, i ):     self.SetValue32( 2, i )
    def SetK32         ( self, i ):     self.SetValue32( 3, i )
    def SetX32         ( self, i ):     self.SetValue32( 0, i )
    def SetY32         ( self, i ):     self.SetValue32( 1, i )
    def SetZ32         ( self, i ):     self.SetValue32( 2, i )
    def SetL32         ( self, i ):     self.SetValue32( 0, i )
    def Seta32         ( self, i ):     self.SetValue32( 1, i )
    def Setb32         ( self, i ):     self.SetValue32( 2, i )
    def Setx32         ( self, i ):     self.SetValue32( 1, i )
    def Sety32         ( self, i ):     self.SetValue32( 2, i )
    def SetAF          ( self, i ):     self.SetAlphaF( i )
    def SetGreyF       ( self, i ):     self.SetValueF( 0, i )
    def SetRedF        ( self, i ):     self.SetValueF( 0, i )
    def SetGreenF      ( self, i ):     self.SetValueF( 1, i )
    def SetBlueF       ( self, i ):     self.SetValueF( 2, i )
    def SetRF          ( self, i ):     self.SetValueF( 0, i )
    def SetGF          ( self, i ):     self.SetValueF( 1, i )
    def SetBF          ( self, i ):     self.SetValueF( 2, i )
    def SetHueF        ( self, i ):     self.SetValueF( 0, i )
    def SetSaturationF ( self, i ):     self.SetValueF( 1, i )
    def SetLightnessF  ( self, i ):     self.SetValueF( 2, i )
    def SetValueF      ( self, i ):     self.SetValueF( 2, i )
    def SetCyanF       ( self, i ):     self.SetValueF( 0, i )
    def SetMagentaF    ( self, i ):     self.SetValueF( 1, i )
    def SetYellowF     ( self, i ):     self.SetValueF( 2, i )
    def SetKeyF        ( self, i ):     self.SetValueF( 3, i )
    def SetLumaF       ( self, i ):     self.SetValueF( 0, i )
    def SetUF          ( self, i ):     self.SetValueF( 1, i )
    def SetVF          ( self, i ):     self.SetValueF( 2, i )
    def SetKF          ( self, i ):     self.SetValueF( 3, i )
    def SetXF          ( self, i ):     self.SetValueF( 0, i )
    def SetYF          ( self, i ):     self.SetValueF( 1, i )
    def SetZF          ( self, i ):     self.SetValueF( 2, i )
    def SetLF          ( self, i ):     self.SetValueF( 0, i )
    def SetaF          ( self, i ):     self.SetValueF( 1, i )
    def SetbF          ( self, i ):     self.SetValueF( 2, i )
    def SetxF          ( self, i ):     self.SetValueF( 1, i )
    def SetyF          ( self, i ):     self.SetValueF( 2, i )
    def SetAD          ( self, i ):     self.SetAlphaD( i )
    def SetGreyD       ( self, i ):     self.SetValueD( 0, i )
    def SetRedD        ( self, i ):     self.SetValueD( 0, i )
    def SetGreenD      ( self, i ):     self.SetValueD( 1, i )
    def SetBlueD       ( self, i ):     self.SetValueD( 2, i )
    def SetRD          ( self, i ):     self.SetValueD( 0, i )
    def SetGD          ( self, i ):     self.SetValueD( 1, i )
    def SetBD          ( self, i ):     self.SetValueD( 2, i )
    def SetHueD        ( self, i ):     self.SetValueD( 0, i )
    def SetSaturationD ( self, i ):     self.SetValueD( 1, i )
    def SetLightnessD  ( self, i ):     self.SetValueD( 2, i )
    def SetValueD      ( self, i ):     self.SetValueD( 2, i )
    def SetCyanD       ( self, i ):     self.SetValueD( 0, i )
    def SetMagentaD    ( self, i ):     self.SetValueD( 1, i )
    def SetYellowD     ( self, i ):     self.SetValueD( 2, i )
    def SetKeyD        ( self, i ):     self.SetValueD( 3, i )
    def SetLumaD       ( self, i ):     self.SetValueD( 0, i )
    def SetUD          ( self, i ):     self.SetValueD( 1, i )
    def SetVD          ( self, i ):     self.SetValueD( 2, i )
    def SetKD          ( self, i ):     self.SetValueD( 3, i )
    def SetXD          ( self, i ):     self.SetValueD( 0, i )
    def SetYD          ( self, i ):     self.SetValueD( 1, i )
    def SetZD          ( self, i ):     self.SetValueD( 2, i )
    def SetLD          ( self, i ):     self.SetValueD( 0, i )
    def SetaD          ( self, i ):     self.SetValueD( 1, i )
    def SetbD          ( self, i ):     self.SetValueD( 2, i )
    def SetxD          ( self, i ):     self.SetValueD( 1, i )
    def SetyD          ( self, i ):     self.SetValueD( 2, i )

