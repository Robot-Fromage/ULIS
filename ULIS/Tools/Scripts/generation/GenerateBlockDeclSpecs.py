tp = [ "uint8", "uint16", "uint32", "float" ]
cm = [ "G", "RGB", "HSL", "HSV", "CMYK", "YUV", "Lab", "XYZ" ]
ea = [ "hasAlpha", "noAlpha" ]
nm = [ "normalized", "typeLimits" ]

variants =    {
    ( "G", "noAlpha"    )       : [ "G" ],
    ( "G", "hasAlpha"   )       : [ "GA", "AG" ],
    ( "RGB", "noAlpha"  )       : [ "RGB", "BGR" ],
    ( "RGB", "hasAlpha" )       : [ "ARGB", "ABGR", "RGBA", "BGRA" ],
    ( "HSL", "hasAlpha" )       : [ "HSLA" ],
    ( "HSV", "hasAlpha" )       : [ "HSVA" ],
    ( "CMYK", "hasAlpha")       : [ "CMYKA" ],
    ( "YUV", "hasAlpha" )       : [ "YUVA" ],
    ( "Lab", "hasAlpha" )       : [ "LabA" ],
    ( "XYZ", "hasAlpha" )       : [ "XYZA" ],
}

def is_decimal( iType ):
    return  iType == "float" or iType == "double"

for icm in cm:
    for itp in tp: 
        for iea in ea:
            for inm in nm:
                if( not is_decimal( itp ) and inm == "normalized" ):
                    continue
                    
                if( is_decimal( itp ) and inm == "typeLimits" ):
                    continue
                    
                var = ( icm, iea )
                if var in variants.keys():
                    for ivar in variants[var]:
                        print( "ULIS_DECLSPEC(" + itp + "," + icm + "," + iea + "," + ivar + "," + inm + ")" )