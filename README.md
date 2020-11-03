# ULIS2
U Library for Imaging Systems

## License
Check LICENSE.md, ULIS2 is not available for commercial use.
|Name               |Link                                                           |License            |
|-------------------|---------------------------------------------------------------|-------------------|
|ULIS2              |https://github.com/Praxinos/ULIS2                              |CC-BY-NC-ND-4.0    |

## Description

        Cross-Platform C++14 Library
        2D Software Rasterizer on CPU
        Build as static or dynamic library
        Very lightweight, good performance
        Generic Digital Image Processing
        Python3x binding

Details:

        Read Write from files for many popular formats
        Image conversions
        Render Text to any image in memory
        Affine Image Transforms
        FX, Convolution, Blur, Noise, ...
        All Standard Pixel Blending Modes
        Many Image Formats, Layouts and Types ( u8, u16, u32, float, double )
        Color Models ( RGB, HSL, HSV, CMY, CMYK, Lab, XYZ, ... )
        Color-Managed Pipelines, Color Profiles Support
        Multithreaded optimisations
        SSE4.2 and AVX2 optimisations
        Support for GPU image processing with OpenCL
        Large Tiled Images ( 300000 px * 300000 px)
        Image Pools and Caches for Optimisation

## History
ULIS was first developped by Clément BERTHAUD ( a.k.a Layl or Robot-Fromage ) on his spare-time in December 2018.  
It was then developped as part of Praxinos, aiming for robust tools for digital image processing.  
ULIS "1.0" was first released internaly for Praxinos at the end of summer 2019, and used in Iliad, a digital painting plugin for UnrealEngine developped by Praxinos.  
ULIS "1.0" had a lot of issues. It was hard to maintain and overused template, resulting in terrible binary sizes for the compiled library ( over 200mo for a release build ).  
Performance was ok but there was a lot of room for improvments.  
The library needed to mature a bit, and the changes to make were so drastic that a separate successor library, ULIS2, was starting development in beginning of January 2020.  
ULIS2 was scheduled for release in February 2020, and comes with a much simpler API, drastically reduced compile times and output binary sizes, better performances, and a lot of additional features such as support for more models, actual support for AVX2 optimisations, color mix, smudge, new blending modes and alpha modes, new interpolation types, etc.  
The build framework was also simplified and extended to conform to the typical cmake build process.  
Support for GPU image processing with OpenCL was introduced.  
Additionally, the library dependencies changed and are now embedded in the repository. The library doesn't rely on preprocessor trickery so much anymore so boost_preprocessor and coal were removed, Qt and OpenCL are now fully optional, and VCL was added for handling SSE / AVX optimisations in a simpler way.  

## Software Requirements Specification ( SRS )

        Linux, Windows or MacOS
        CMake
        Git
        C++ compiler with support for C++14 or greater

## Library Dependencies
Dependencies are redistributed directly within the repository under the 3rdparty directory.

|Dependency         |Link                                                           |License            |
|-------------------|---------------------------------------------------------------|-------------------|
|clip               |https://github.com/dacap/clip                                  |MIT                |
|cppfs              |https://github.com/cginternals/cppfs                           |MIT                |
|eigen              |https://gitlab.com/libeigen/eigen                              |MPL2               |
|FeatureDetector    |https://github.com/Mysticial/FeatureDetector                   |CC0 1.0 Universal  |
|freetype2          |https://github.com/aseprite/freetype2                          |FTL ( BSD-like )   |
|glm                |https://github.com/g-truc/glm                                  |MIT                |
|Little-CMS         |https://github.com/mm2/Little-CMS                              |MIT                |
|OCL-SDK            |https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases   |-                  |
|static_math        |https://github.com/Morwenn/static_math                         |MIT                |
|stb_image          |https://github.com/nothings/stb                                |MIT                |
|stb_image_write    |https://github.com/nothings/stb                                |MIT                |
|VCL                |https://github.com/vectorclass/version1                        |Apache 2.0         |

Additional Optional library can be used with ULIS2 to test samples programs or build the python binding:

        Qt5.x
        Python3.x

## Getting Started

        git clone https://github.com/Praxinos/ULIS2
        mkdir ULIS2_Build
        cd ULIS2_Build
        cmake -G ../ULIS2

Sample scripts are also available in the repository to help with the build process.

## ULIS2 for UnrealEngine ThirdParty Module
A `ULIS2.Build.cs` script is provided within the repository.
ULIS2 GPU capabilities and interop with Direct3D or OpenGL in the context of a generic rendering hardware interface is still a work in progress.

## Examples
This is a small sample illustrating image loading, copy and blend operatios. Check samples to get a gist of how to use ULIS2, more details and specific functions.

```cpp
#include <ULIS2>
using namespace ::ul2;

int main() {
    FThreadPool  threadPool;
    uint32 perfIntent = ULIS2_PERF_MT | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    std::string pathBase = "base_160.png";
    std::string pathOver = "over_160.png";
    FBlock* blockBase = XLoadFromFile( &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, pathBase, ULIS2_FORMAT_RGBA8 );
    FBlock* blockOver = XLoadFromFile( &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, pathOver, ULIS2_FORMAT_RGBA8 );
    Fence( threadPool );
    FRect sourceRect = blockBase->Rect();
    int w = sourceRect.w * 8;
    int h = sourceRect.h * 5;
    FBlock* blockCanvas = new  FBlock( w, h, ULIS2_FORMAT_RGBA8 );
    for( int i = 0; i < NUM_BLENDING_MODES; ++i ) {
        int x = ( i % 8 ) * sourceRect.w;
        int y = ( i / 8 ) * sourceRect.h;
        Copy(   &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB, blockBase, blockCanvas, sourceRect, FVec2I( x, y ) );
        Blend(  &threadPool, ULIS2_NONBLOCKING, perfIntent, host, ULIS2_NOCB, blockOver, blockCanvas, sourceRect, FVec2F( x, y ), ULIS2_NOAA, static_cast< eBlendingMode >( i ), AM_NORMAL, 0.5f );
    }
    Fence( threadPool );
    delete  blockBase;
    delete  blockOver;
    delete  blockCanvas;
    return  0;
}
```

