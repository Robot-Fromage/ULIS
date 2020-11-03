from enum import Enum
# Enum Type
TYPE_UINT8      = 0
TYPE_UINT16     = 1
TYPE_UINT32     = 2
TYPE_UFLOAT     = 3
TYPE_UDOUBLE    = 4
TYPE_INVALID    = 5
kwType = [ "UINT8", "UINT16", "UINT32", "UFLOAT", "UDOUBLE" ]
kwTypeA = [ "8", "16", "32", "F", "D" ]

# Enum ColorModel
CM_ANY  = 0
CM_GREY = 1
CM_RGB  = 2
CM_HSV  = 3
CM_HSL  = 4
CM_CMY  = 5
CM_CMYK = 6
CM_YUV  = 7
CM_Lab  = 8
CM_XYZ  = 9
CM_Yxy  = 10
kwColorModel = [ "ANY", "Grey", "RGB", "HSV", "HSL", "CMY", "CMYK", "YUV", "Lab", "XYZ", "Yxy" ]

