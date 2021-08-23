import webbrowser
from subprocess import Popen, PIPE, check_output
from os.path    import abspath, dirname, join
from time       import sleep
import math
import subprocess
import os
import fileinput

numWorkers = os.cpu_count()
criticalError = 0xB00BA420

benchmark_msvc_path   = abspath( join( dirname( __file__ ), 'bin/msvc/Benchmark.exe' ) )
benchmark_clang_path  = abspath( join( dirname( __file__ ), 'bin/clang/Benchmark.exe' ) )
benchmark_gcc_path  = abspath( join( dirname( __file__ ), 'bin/gcc/Benchmark.exe' ) )
cpuinfo_path  = abspath( join( dirname( __file__ ), 'bin/gcc/CPUDeviceInfo.exe' ) )
gpuinfo_path  = abspath( join( dirname( __file__ ), 'bin/gcc/GPUDeviceInfo.exe' ) )

formats = { "G8"        : 65808     , "RGB16"     : 131633      , "ABGRD"     : 569908      , "HSVF"      : 267059      , "ALSH16"    : 173105
          , "GA8"       : 98576     , "BGR16"     : 139825      , "ARGBD"     : 578100      , "VSHF"      : 275251      , "AHSL16"    : 181297
          , "AG8"       : 114960    , "RGBA16"    : 164401      , "BGRAD"     : 586292      , "HSVAF"     : 299827      , "LSHA16"    : 189489
          , "G16"       : 131345    , "ABGR16"    : 172593      , "HSV8"      : 66352       , "AVSHF"     : 308019      , "HSL32"     : 263218
          , "GA16"      : 164113    , "ARGB16"    : 180785      , "VSH8"      : 74544       , "AHSVF"     : 316211      , "LSH32"     : 271410
          , "AG16"      : 180497    , "BGRA16"    : 188977      , "HSVA8"     : 99120       , "VSHAF"     : 324403      , "HSLA32"    : 295986
          , "G32"       : 262418    , "RGB32"     : 262706      , "AVSH8"     : 107312      , "HSVD"      : 529204      , "ALSH32"    : 304178
          , "GA32"      : 295186    , "BGR32"     : 270898      , "AHSV8"     : 115504      , "VSHD"      : 537396      , "AHSL32"    : 312370
          , "AG32"      : 311570    , "RGBA32"    : 295474      , "VSHA8"     : 123696      , "HSVAD"     : 561972      , "LSHA32"    : 320562
          , "GF"        : 266515    , "ABGR32"    : 303666      , "HSV16"     : 131889      , "AVSHD"     : 570164      , "HSLF"      : 267315
          , "GAF"       : 299283    , "ARGB32"    : 311858      , "VSH16"     : 140081      , "AHSVD"     : 578356      , "LSHF"      : 275507
          , "AGF"       : 315667    , "BGRA32"    : 320050      , "HSVA16"    : 164657      , "VSHAD"     : 586548      , "HSLAF"     : 300083
          , "GD"        : 528660    , "RGBF"      : 266803      , "AVSH16"    : 172849      , "HSL8"      : 66608       , "ALSHF"     : 308275
          , "GAD"       : 561428    , "BGRF"      : 274995      , "AHSV16"    : 181041      , "LSH8"      : 74800       , "AHSLF"     : 316467
          , "AGD"       : 577812    , "RGBAF"     : 299571      , "VSHA16"    : 189233      , "HSLA8"     : 99376       , "LSHAF"     : 324659
          , "RGB8"      : 66096     , "ABGRF"     : 307763      , "HSV32"     : 262962      , "ALSH8"     : 107568      , "HSLD"      : 529460
          , "BGR8"      : 74288     , "ARGBF"     : 315955      , "VSH32"     : 271154      , "AHSL8"     : 115760      , "LSHD"      : 537652
          , "RGBA8"     : 98864     , "BGRAF"     : 324147      , "HSVA32"    : 295730      , "LSHA8"     : 123952      , "HSLAD"     : 562228
          , "ABGR8"     : 107056    , "RGBD"      : 528948      , "AVSH32"    : 303922      , "HSL16"     : 132145      , "ALSHD"     : 570420
          , "ARGB8"     : 115248    , "BGRD"      : 537140      , "AHSV32"    : 312114      , "LSH16"     : 140337      , "AHSLD"     : 578612
          , "BGRA8"     : 123440    , "RGBAD"     : 561716      , "VSHA32"    : 320306      , "HSLA16"    : 164913      , "LSHAD"     : 586804

          , "CMY8"      : 66864     , "ACMYF"     : 316723      , "AKYMC16"   : 173633      , "CMYKAD"    : 562756      , "YUVF"      : 268083
          , "YMC8"      : 75056     , "YMCAF"     : 324915      , "KYMCA16"   : 190017      , "ACMYKD"    : 579140      , "VUYF"      : 276275
          , "CMYA8"     : 99632     , "CMYD"      : 529716      , "CMYK32"    : 263746      , "AKYMCD"    : 570948      , "YUVAF"     : 300851
          , "AYMC8"     : 107824    , "YMCD"      : 537908      , "KCMY32"    : 280130      , "KYMCAD"    : 587332      , "AVUYF"     : 309043
          , "ACMY8"     : 116016    , "CMYAD"     : 562484      , "KYMC32"    : 271938      , "YUV8"      : 67376       , "AYUVF"     : 317235
          , "YMCA8"     : 124208    , "AYMCD"     : 570676      , "YMCK32"    : 288322      , "VUY8"      : 75568       , "VUYAF"     : 325427
          , "CMY16"     : 132401    , "ACMYD"     : 578868      , "CMYKA32"   : 296514      , "YUVA8"     : 100144      , "YUVD"      : 530228
          , "YMC16"     : 140593    , "YMCAD"     : 587060      , "ACMYK32"   : 312898      , "AVUY8"     : 108336      , "VUYD"      : 538420
          , "CMYA16"    : 165169    , "CMYK8"     : 67136       , "AKYMC32"   : 304706      , "AYUV8"     : 116528      , "YUVAD"     : 562996
          , "AYMC16"    : 173361    , "KCMY8"     : 83520       , "KYMCA32"   : 321090      , "VUYA8"     : 124720      , "AVUYD"     : 571188
          , "ACMY16"    : 181553    , "KYMC8"     : 75328       , "CMYKF"     : 267843      , "YUV16"     : 132913      , "AYUVD"     : 579380
          , "YMCA16"    : 189745    , "YMCK8"     : 91712       , "KCMYF"     : 284227      , "VUY16"     : 141105      , "VUYAD"     : 587572
          , "CMY32"     : 263474    , "CMYKA8"    : 99904       , "KYMCF"     : 276035      , "YUVA16"    : 165681      , "Lab8"      : 67632
          , "YMC32"     : 271666    , "ACMYK8"    : 116288      , "YMCKF"     : 292419      , "AVUY16"    : 173873      , "baL8"      : 75824
          , "CMYA32"    : 296242    , "AKYMC8"    : 108096      , "CMYKAF"    : 300611      , "AYUV16"    : 182065      , "LabA8"     : 100400
          , "AYMC32"    : 304434    , "KYMCA8"    : 124480      , "ACMYKF"    : 316995      , "VUYA16"    : 190257      , "AbaL8"     : 108592
          , "ACMY32"    : 312626    , "CMYK16"    : 132673      , "AKYMCF"    : 308803      , "YUV32"     : 263986      , "ALab8"     : 116784
          , "YMCA32"    : 320818    , "KCMY16"    : 149057      , "KYMCAF"    : 325187      , "VUY32"     : 272178      , "baLA8"     : 124976
          , "CMYF"      : 267571    , "KYMC16"    : 140865      , "CMYKD"     : 529988      , "YUVA32"    : 296754      , "Lab16"     : 133169
          , "YMCF"      : 275763    , "YMCK16"    : 157249      , "KCMYD"     : 546372      , "AVUY32"    : 304946      , "baL16"     : 141361
          , "CMYAF"     : 300339    , "CMYKA16"   : 165441      , "KYMCD"     : 538180      , "AYUV32"    : 313138      , "LabA16"    : 165937
          , "AYMCF"     : 308531    , "ACMYK16"   : 181825      , "YMCKD"     : 554564      , "VUYA32"    : 321330      , "AbaL16"    : 174129

          , "ALab16"    : 182321    , "XYZA8"     : 100656      , "XYZD"      : 530740      , "AYxy32"    : 313906
          , "baLA16"    : 190513    , "AZYX8"     : 108848      , "ZYXD"      : 538932      , "yxYA32"    : 322098
          , "Lab32"     : 264242    , "AXYZ8"     : 117040      , "XYZAD"     : 563508      , "YxyF"      : 268851
          , "baL32"     : 272434    , "ZYXA8"     : 125232      , "AZYXD"     : 571700      , "yxYF"      : 277043
          , "LabA32"    : 297010    , "XYZ16"     : 133425      , "AXYZD"     : 579892      , "YxyAF"     : 301619
          , "AbaL32"    : 305202    , "ZYX16"     : 141617      , "ZYXAD"     : 588084      , "AyxYF"     : 309811
          , "ALab32"    : 313394    , "XYZA16"    : 166193      , "Yxy8"      : 68144       , "AYxyF"     : 318003
          , "baLA32"    : 321586    , "AZYX16"    : 174385      , "yxY8"      : 76336       , "yxYAF"     : 326195
          , "LabF"      : 268339    , "AXYZ16"    : 182577      , "YxyA8"     : 100912      , "YxyD"      : 530996
          , "baLF"      : 276531    , "ZYXA16"    : 190769      , "AyxY8"     : 109104      , "yxYD"      : 539188
          , "LabAF"     : 301107    , "XYZ32"     : 264498      , "AYxy8"     : 117296      , "YxyAD"     : 563764
          , "AbaLF"     : 309299    , "ZYX32"     : 272690      , "yxYA8"     : 125488      , "AyxYD"     : 571956
          , "ALabF"     : 317491    , "XYZA32"    : 297266      , "Yxy16"     : 133681      , "AYxyD"     : 580148
          , "baLAF"     : 325683    , "AZYX32"    : 305458      , "yxY16"     : 141873      , "yxYAD"     : 588340
          , "LabD"      : 530484    , "AXYZ32"    : 313650      , "YxyA16"    : 166449
          , "baLD"      : 538676    , "ZYXA32"    : 321842      , "AyxY16"    : 174641
          , "LabAD"     : 563252    , "XYZF"      : 268595      , "AYxy16"    : 182833
          , "AbaLD"     : 571444    , "ZYXF"      : 276787      , "yxYA16"    : 191025
          , "ALabD"     : 579636    , "XYZAF"     : 301363      , "Yxy32"     : 264754
          , "baLAD"     : 587828    , "AZYXF"     : 309555      , "yxY32"     : 272946
          , "XYZ8"      : 67888     , "AXYZF"     : 317747      , "YxyA32"    : 297522
          , "ZYX8"      : 76080     , "ZYXAF"     : 325939      , "AyxY32"    : 305714 }

htmlHead        ="<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"utf-8\"><title>ULIS3 Benchmark</title><style>body{background-color:#23241f;color:#f8f8d1;font-family:Lucida Console;margin:0;padding:0;overflow-x:hidden}table,td,th{border-collapse:collapse;text-align:left;padding:0;margin:0}.info{background-color:#131310;font-size:12px}.infoTitle{font-size:16px;padding:5px;width:100%;text-align:center;text-align:center;background:#23241f;background:linear-gradient(0deg,#23241f 0,rgba(78,84,52,.2) 100%);border-top:1px solid #4e5434}.CPUInfo{height:600px;overflow-y:scroll;overflow-x:visible;white-space:pre-wrap;vertical-align:top;padding:0;margin:0;display:inline-block;background-color:#131310;color:#ddd;width:50%}.GPUInfo{height:600px;overflow-y:scroll;overflow-x:visible;white-space:pre-wrap;vertical-align:top;padding:0;margin:0;display:inline-block;background-color:#131310;color:#ddd;width:50%}.benchmarkTitle{text-align:center;background:#23241f;background:linear-gradient(0deg,#23241f 0,rgba(78,84,52,.2) 100%);border-top:1px solid #4e5434}td,th{padding:5px}.benchmarkTable{width:100%}.benchmarkID{background-color:#131310}th.benchmarkOP{text-align:center}.benchmarkOP{background-color:#470c00}.benchmarkMSVC{background-color:#450029}.benchmarkCLANG{background-color:#751900}.benchmarkGCC{background-color:#61000f}.benchmarkTimeMS{text-align:center}td.benchmarkOP{background-color:#6c291c}td.benchmarkMSVC{background-color:#6a1c4a}td.benchmarkCLANG{background-color:#9a3115}td.benchmarkGCC{background-color:#861829}td.benchmarkEntry{color:rgba(255,255,255,.5)}td.benchmarkEntryOPSize64.benchmarkOP{background-color:#47231c}td.benchmarkEntryOPSize64.benchmarkTimeMS.benchmarkMSVC{background-color:#453b41}td.benchmarkEntryOPSize64.benchmarkTimeMS.benchmarkCLANG{background-color:#753624}td.benchmarkEntryOPSize64.benchmarkTimeMS.benchmarkGCC{background-color:#602b33}td.benchmarkEntryOPSize128.benchmarkOP{background-color:#4a3919}td.benchmarkEntryOPSize128.benchmarkTimeMS.benchmarkMSVC{background-color:#48543e}td.benchmarkEntryOPSize128.benchmarkTimeMS.benchmarkCLANG{background-color:#784f21}td.benchmarkEntryOPSize128.benchmarkTimeMS.benchmarkGCC{background-color:#634430}td.benchmarkEntryOPSize256.benchmarkOP{background-color:#3e3c1d}td.benchmarkEntryOPSize256.benchmarkTimeMS.benchmarkMSVC{background-color:#3a5743}td.benchmarkEntryOPSize256.benchmarkTimeMS.benchmarkCLANG{background-color:#6a5225}td.benchmarkEntryOPSize256.benchmarkTimeMS.benchmarkGCC{background-color:#554735}td.benchmarkEntryOPSize512.benchmarkOP{background-color:#1d3521}td.benchmarkEntryOPSize512.benchmarkTimeMS.benchmarkMSVC{background-color:#154f47}td.benchmarkEntryOPSize512.benchmarkTimeMS.benchmarkCLANG{background-color:#454a2a}td.benchmarkEntryOPSize512.benchmarkTimeMS.benchmarkGCC{background-color:#303f39}td.benchmarkEntryOPSize1024.benchmarkOP{background-color:#1d3331}td.benchmarkEntryOPSize1024.benchmarkTimeMS.benchmarkMSVC{background-color:#154d59}td.benchmarkEntryOPSize1024.benchmarkTimeMS.benchmarkCLANG{background-color:#45473b}td.benchmarkEntryOPSize1024.benchmarkTimeMS.benchmarkGCC{background-color:#303d4b}td.benchmarkEntryOPSize2048.benchmarkOP{background-color:#1d2730}td.benchmarkEntryOPSize2048.benchmarkTimeMS.benchmarkMSVC{background-color:#154058}td.benchmarkEntryOPSize2048.benchmarkTimeMS.benchmarkCLANG{background-color:#453a3b}td.benchmarkEntryOPSize2048.benchmarkTimeMS.benchmarkGCC{background-color:#303049}td.benchmarkEntryOPSize4096.benchmarkOP{background-color:#281f2c}td.benchmarkEntryOPSize4096.benchmarkTimeMS.benchmarkMSVC{background-color:#213754}td.benchmarkEntryOPSize4096.benchmarkTimeMS.benchmarkCLANG{background-color:#513237}td.benchmarkEntryOPSize4096.benchmarkTimeMS.benchmarkGCC{background-color:#3c2745}td.benchmarkEntryOPSize8192.benchmarkOP{background-color:#381d2a}td.benchmarkEntryOPSize8192.benchmarkTimeMS.benchmarkMSVC{background-color:#333452}td.benchmarkEntryOPSize8192.benchmarkTimeMS.benchmarkCLANG{background-color:#632f35}td.benchmarkEntryOPSize8192.benchmarkTimeMS.benchmarkGCC{background-color:#4e2444}td.benchmarkEntryOPSize16384.benchmarkOP{background-color:#1d1713}td.benchmarkEntryOPSize16384.benchmarkTimeMS.benchmarkMSVC{background-color:#152d37}td.benchmarkEntryOPSize16384.benchmarkTimeMS.benchmarkCLANG{background-color:#45281a}td.benchmarkEntryOPSize16384.benchmarkTimeMS.benchmarkGCC{background-color:#301d29}</style><script>function AutoRefresh( t ) {setTimeout(\"location.reload(false);\", t);}</script></head>"
htmlBodyBegin   ="<body onload = \"JavaScript:AutoRefresh({});\">"
htmlInfoBegin   ="<div class=\"info\"><div class=\"infoTitle\">Local Machine Features</div><div class=\"CPUInfo\">"
cpuinfo         = subprocess.run( cpuinfo_path, capture_output=True)
cpuinfo         = str( cpuinfo.stdout, 'utf8' ).replace( "\r", "" )
htmlInfoInter   ="</div><div class=\"GPUInfo\">"
gpuinfo         = subprocess.run( gpuinfo_path, capture_output=True)
gpuinfo         = str( gpuinfo.stdout, 'utf8' ).replace( "\r", "" )
htmlInfoEnd     ="</div></div>"
htmlTableBegin  ="<div id=\"mainWrapper\"> <table class=\"benchmarkTable\"> <thead> <tr> <th class=\"benchmarkHead benchmarkTitle\" colspan=\"15\">ULIS3 Benchmark</th> </tr><tr> <td class=\"benchmarkHead benchmarkID\" colspan=\"15\">Machine ID</td></tr><tr> <th class=\"benchmarkHead benchmarkOP \" colspan=\"6\">Operation</th> <th class=\"benchmarkHead benchmarkMSVC benchmarkTimeMS \" colspan=\"3\">MSVC</th> <th class=\"benchmarkHead benchmarkCLANG benchmarkTimeMS \" colspan=\"3\">CLANG</th> <th class=\"benchmarkHead benchmarkGCC benchmarkTimeMS \" colspan=\"3\">GCC</th> </tr><tr> <td class=\"benchmarkHead benchmarkOP benchmarkName \" colspan=\"1\">Name</td><td class=\"benchmarkHead benchmarkOP benchmarkFormat \" colspan=\"1\">Format</td><td class=\"benchmarkHead benchmarkOP benchmarkThreads \" colspan=\"1\">Threads</td><td class=\"benchmarkHead benchmarkOP benchmarkRepeat \" colspan=\"1\">Repeat</td><td class=\"benchmarkHead benchmarkOP benchmarkSize \" colspan=\"1\">Size</td><td class=\"benchmarkHead benchmarkOP benchmarkExtra \" colspan=\"1\">Parameters</td><td class=\"benchmarkHead benchmarkMSVC benchmarkTimeMS benchmarkMEM \" colspan=\"1\">MEM</td><td class=\"benchmarkHead benchmarkMSVC benchmarkTimeMS benchmarkSSE \" colspan=\"1\">SSE42</td><td class=\"benchmarkHead benchmarkMSVC benchmarkTimeMS benchmarkAVX \" colspan=\"1\">AVX2</td><td class=\"benchmarkHead benchmarkCLANG benchmarkTimeMS benchmarkMEM \" colspan=\"1\">MEM</td><td class=\"benchmarkHead benchmarkCLANG benchmarkTimeMS benchmarkSSE \" colspan=\"1\">SSE42</td><td class=\"benchmarkHead benchmarkCLANG benchmarkTimeMS benchmarkAVX \" colspan=\"1\">AVX2</td><td class=\"benchmarkHead benchmarkGCC benchmarkTimeMS benchmarkMEM \" colspan=\"1\">MEM</td><td class=\"benchmarkHead benchmarkGCC benchmarkTimeMS benchmarkSSE \" colspan=\"1\">SSE42</td><td class=\"benchmarkHead benchmarkGCC benchmarkTimeMS benchmarkAVX \" colspan=\"1\">AVX2</td></tr></thead> <tbody>"
htmlEnd         ="</tbody></table></div></body></html>"
htmlEntry = """
<tr>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkOP     benchmarkName       " >{op}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkOP     benchmarkFormat     " >{format}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkOP     benchmarkThreads    " >{threads}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkOP     benchmarkRepeat     " >{repeat}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkOP     benchmarkSize       " >{size}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkOP     benchmarkExtra      " >{parameters}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkMSVC       " >{MSVCTimeMEM}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkMSVC       " >{MSVCTimeSSE}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkMSVC       " >{MSVCTimeAVX}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkCLANG      " >{CLANGTimeMEM}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkCLANG      " >{CLANGTimeSSE}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkCLANG      " >{CLANGTimeAVX}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkGCC        " >{GCCTimeMEM}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkGCC        " >{GCCTimeSSE}</td>
    <td class="benchmarkEntry   benchmarkEntryOPSize{size} benchmarkTimeMS benchmarkGCC        " >{GCCTimeAVX}</td>
</tr>"""

reloadTime = 5000
fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
fileObjectBenchmarkHtml.truncate(0)
fileObjectBenchmarkHtml.write( htmlHead )
fileObjectBenchmarkHtml.write( htmlBodyBegin.format( reloadTime ) )
fileObjectBenchmarkHtml.write( htmlInfoBegin )
fileObjectBenchmarkHtml.write( cpuinfo )
fileObjectBenchmarkHtml.write( htmlInfoInter )
fileObjectBenchmarkHtml.write( gpuinfo )
fileObjectBenchmarkHtml.write( htmlInfoEnd )
fileObjectBenchmarkHtml.write( htmlTableBegin )
fileObjectBenchmarkHtml.close()

print( "ULIS3 Benchmark running, don't close this window !" )
print( "get visual feedback @" + join( dirname( __file__ ), "benchmark.html" ) )
webbrowser.open('benchmark.html', new=2)

sizes   = [ 64,     128,    256,    512,    1024,   2048,   4096,   8192,   16384   ]
repeats = [ 16384,  8192,   4096,   2048,   1024,   512,    256,    128,    64      ]
repeatModifier = 0.5
numBlendingModes    = 40
numAlphaModes       = 9
op      = [ "clear", "fill", "blend", "copy", "conv" ]
subWorker = numWorkers
workers = [numWorkers]

while subWorker > 1:
    subWorker = int( subWorker / 2 )
    workers.append( subWorker )
workers = workers[::-1]

# Test Clear Fill Copy RGBA8
for _op in [ "clear", "fill", "copy" ]:
    for i in range( len(sizes) ):
        for _threads in workers:
            modRepeat = math.ceil( repeats[i] * repeatModifier )
            timeMSVCMEM     = Popen( [ benchmark_msvc_path,     _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem" ], stdout=PIPE ).wait()     / modRepeat
            timeMSVCSSE     = Popen( [ benchmark_msvc_path,     _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse" ], stdout=PIPE ).wait()     / modRepeat
            timeMSVCAVX     = Popen( [ benchmark_msvc_path,     _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx" ], stdout=PIPE ).wait()     / modRepeat
            timeCLANGMEM    = Popen( [ benchmark_clang_path,    _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem" ], stdout=PIPE ).wait()     / modRepeat
            timeCLANGSSE    = Popen( [ benchmark_clang_path,    _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse" ], stdout=PIPE ).wait()     / modRepeat
            timeCLANGAVX    = Popen( [ benchmark_clang_path,    _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx" ], stdout=PIPE ).wait()     / modRepeat
            timeGCCMEM      = Popen( [ benchmark_gcc_path,      _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem" ], stdout=PIPE ).wait()     / modRepeat
            timeGCCSSE      = Popen( [ benchmark_gcc_path,      _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse" ], stdout=PIPE ).wait()     / modRepeat
            timeGCCAVX      = Popen( [ benchmark_gcc_path,      _op, str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx" ], stdout=PIPE ).wait()     / modRepeat
            fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
            fileObjectBenchmarkHtml.write( htmlEntry.format( size           = sizes[i]
                                                           , op             = _op
                                                           , format         = "RGBA8"
                                                           , threads        = _threads
                                                           , repeat         = modRepeat
                                                           , parameters     = "-"
                                                           , MSVCTimeMEM    = format( timeMSVCMEM, '.6f')
                                                           , MSVCTimeSSE    = format( timeMSVCSSE, '.6f')
                                                           , MSVCTimeAVX    = format( timeMSVCAVX, '.6f')
                                                           , CLANGTimeMEM   = format( timeCLANGMEM, '.6f')
                                                           , CLANGTimeSSE   = format( timeCLANGSSE, '.6f')
                                                           , CLANGTimeAVX   = format( timeCLANGAVX, '.6f')
                                                           , GCCTimeMEM     = format( timeGCCMEM, '.6f' )
                                                           , GCCTimeSSE     = format( timeGCCSSE, '.6f' )
                                                           , GCCTimeAVX     = format( timeGCCAVX, '.6f' ) ) )
            fileObjectBenchmarkHtml.close()

# Test Conv RGBA8 To LabAf
for i in range( len(sizes) ):
    for _threads in workers:
        modRepeat = math.ceil( repeats[i] * repeatModifier )
        timeMSVCMEM     = Popen( [ benchmark_msvc_path,     "conv", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", str( formats["LabAF"] ) ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGMEM    = Popen( [ benchmark_clang_path,    "conv", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", str( formats["LabAF"] ) ], stdout=PIPE ).wait()     / modRepeat
        timeGCCMEM      = Popen( [ benchmark_gcc_path,      "conv", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", str( formats["LabAF"] ) ], stdout=PIPE ).wait()     / modRepeat
        fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
        fileObjectBenchmarkHtml.write( htmlEntry.format( size           = sizes[i]
                                                       , op             ="conv"
                                                       , format         ="RGBA8"
                                                       , threads        = _threads
                                                       , repeat         = modRepeat
                                                       , parameters     = "To LabAf"
                                                       , MSVCTimeMEM    = format( timeMSVCMEM, '.6f')
                                                       , MSVCTimeSSE    = "-"
                                                       , MSVCTimeAVX    = "-"
                                                       , CLANGTimeMEM   = format( timeCLANGMEM, '.6f')
                                                       , CLANGTimeSSE   = "-"
                                                       , CLANGTimeAVX   = "-"
                                                       , GCCTimeMEM     = format( timeGCCMEM, '.6f')
                                                       , GCCTimeSSE     = "-"
                                                       , GCCTimeAVX     = "-" ) )
        fileObjectBenchmarkHtml.close()

#blend noAA
for i in range( len(sizes) ):
    for _threads in workers:
        modRepeat = math.ceil( repeats[i] * repeatModifier )
        timeMSVCMEM     = Popen( [ benchmark_msvc_path,     "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeMSVCSSE     = Popen( [ benchmark_msvc_path,     "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeMSVCAVX     = Popen( [ benchmark_msvc_path,     "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGMEM    = Popen( [ benchmark_clang_path,    "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGSSE    = Popen( [ benchmark_clang_path,    "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGAVX    = Popen( [ benchmark_clang_path,    "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCMEM      = Popen( [ benchmark_gcc_path,      "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCSSE      = Popen( [ benchmark_gcc_path,      "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCAVX      = Popen( [ benchmark_gcc_path,      "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0", "0", "0" ], stdout=PIPE ).wait()     / modRepeat
        fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
        fileObjectBenchmarkHtml.write( htmlEntry.format( size           = sizes[i]
                                                       , op             = "blend"
                                                       , format         = "RGBA8"
                                                       , threads        = _threads
                                                       , repeat         = modRepeat
                                                       , parameters     = "BM_NORMAL | AM_NORMAL | NO_AA"
                                                       , MSVCTimeMEM    = format( timeMSVCMEM, '.6f')
                                                       , MSVCTimeSSE    = format( timeMSVCSSE, '.6f')
                                                       , MSVCTimeAVX    = format( timeMSVCAVX, '.6f')
                                                       , CLANGTimeMEM   = format( timeCLANGMEM, '.6f')
                                                       , CLANGTimeSSE   = format( timeCLANGSSE, '.6f')
                                                       , CLANGTimeAVX   = format( timeCLANGAVX, '.6f')
                                                       , GCCTimeMEM     = format( timeGCCMEM, '.6f' )
                                                       , GCCTimeSSE     = format( timeGCCSSE, '.6f' )
                                                       , GCCTimeAVX     = format( timeGCCAVX, '.6f' ) ) )
        fileObjectBenchmarkHtml.close()

#blend yesAA
for i in range( len(sizes) ):
    for _threads in workers:
        modRepeat = math.ceil( repeats[i] * repeatModifier )
        timeMSVCMEM     = Popen( [ benchmark_msvc_path,     "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeMSVCSSE     = Popen( [ benchmark_msvc_path,     "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeMSVCAVX     = Popen( [ benchmark_msvc_path,     "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGMEM    = Popen( [ benchmark_clang_path,    "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGSSE    = Popen( [ benchmark_clang_path,    "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGAVX    = Popen( [ benchmark_clang_path,    "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCMEM      = Popen( [ benchmark_gcc_path,      "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCSSE      = Popen( [ benchmark_gcc_path,      "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCAVX      = Popen( [ benchmark_gcc_path,      "blend", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
        fileObjectBenchmarkHtml.write( htmlEntry.format( size           = sizes[i]
                                                       , op             = "blend"
                                                       , format         = "RGBA8"
                                                       , threads        = _threads
                                                       , repeat         = modRepeat
                                                       , parameters     = "BM_NORMAL | AM_NORMAL | YES_AA"
                                                       , MSVCTimeMEM    = format( timeMSVCMEM, '.6f')
                                                       , MSVCTimeSSE    = format( timeMSVCSSE, '.6f')
                                                       , MSVCTimeAVX    = format( timeMSVCAVX, '.6f')
                                                       , CLANGTimeMEM   = format( timeCLANGMEM, '.6f')
                                                       , CLANGTimeSSE   = format( timeCLANGSSE, '.6f')
                                                       , CLANGTimeAVX   = format( timeCLANGAVX, '.6f')
                                                       , GCCTimeMEM     = format( timeGCCMEM, '.6f' )
                                                       , GCCTimeSSE     = format( timeGCCSSE, '.6f' )
                                                       , GCCTimeAVX     = format( timeGCCAVX, '.6f' ) ) )
        fileObjectBenchmarkHtml.close()

#transform
for i in range( len(sizes) ):
    for _threads in workers:
        modRepeat = math.ceil( repeats[i] * repeatModifier )
        timeMSVCMEM     = Popen( [ benchmark_msvc_path,     "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeMSVCSSE     = Popen( [ benchmark_msvc_path,     "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeMSVCAVX     = Popen( [ benchmark_msvc_path,     "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGMEM    = Popen( [ benchmark_clang_path,    "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGSSE    = Popen( [ benchmark_clang_path,    "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeCLANGAVX    = Popen( [ benchmark_clang_path,    "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCMEM      = Popen( [ benchmark_gcc_path,      "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "mem", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCSSE      = Popen( [ benchmark_gcc_path,      "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "sse", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        timeGCCAVX      = Popen( [ benchmark_gcc_path,      "transform", str( formats["RGBA8"] ), str( _threads ), str( modRepeat ), str( sizes[i] ), "avx", "0.5", "0", "0", "0", "0.5", "0", "0", "0", "1" ], stdout=PIPE ).wait()     / modRepeat
        fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
        fileObjectBenchmarkHtml.write( htmlEntry.format( size           = sizes[i]
                                                       , op             = "conv"
                                                       , format         = "RGBA8"
                                                       , threads        = _threads
                                                       , repeat         = modRepeat
                                                       , parameters     = "BM_NORMAL | AM_NORMAL | NO_AA"
                                                       , MSVCTimeMEM    = format( timeMSVCMEM, '.6f')
                                                       , MSVCTimeSSE    = format( timeMSVCSSE, '.6f')
                                                       , MSVCTimeAVX    = format( timeMSVCAVX, '.6f')
                                                       , CLANGTimeMEM   = format( timeCLANGMEM, '.6f')
                                                       , CLANGTimeSSE   = format( timeCLANGSSE, '.6f')
                                                       , CLANGTimeAVX   = format( timeCLANGAVX, '.6f')
                                                       , GCCTimeMEM     = format( timeGCCMEM, '.6f' )
                                                       , GCCTimeSSE     = format( timeGCCSSE, '.6f' )
                                                       , GCCTimeAVX     = format( timeGCCAVX, '.6f' ) ) )
        fileObjectBenchmarkHtml.close()

with fileinput.FileInput('benchmark.html', inplace=True ) as file:
    for line in file:
        print(line.replace(htmlBodyBegin.format( reloadTime ), "<body>"), end='')

fileObjectBenchmarkHtml = open('benchmark.html', 'a+')
fileObjectBenchmarkHtml.write( htmlEnd )
fileObjectBenchmarkHtml.close()
print( "ULIS3 Benchmark finished, you can close this window, backup benchmark.html to keep results." )
os.system("pause")