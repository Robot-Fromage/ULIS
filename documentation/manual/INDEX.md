## ‎‎‎ULIS Documentation
![](meta/image/logo/github.png)
ㅤ  
ㅤ  

---

## Overview
Welcome to the official ULIS documentation.  
ULIS ( Utility Library for Imaging Systems ) is a library written in C++, it is aimed at software rasterization and digital image processing.  
The library is young and development is ongoing, but it is already powerful enough to power softwares used in the real world such as **ILIAD**, and to compete with other image processing libraries. The goal of ULIS is to provide a single dependency for applications with interactive drawing capabilities and performance in mind. It features a lot of classical image processing algorithms, as well as an advanced asynchronous programming model. All features are designed to take advantage of parallelism on multicore systems, and use the latest available SIMD instructions sets whenever available ( AVX2, SSE4.2 ). ULIS also strives to stay a small dependency with small binary size, that is easy to build and integrate into other projects with CMake.  
The ULIS repository and all related materials are licensed under a proprietary license. See the **EULA** or **CLUF** at the root of the repository, or contact Praxinos for more informations.

## Links
- [How to build ULIS](md_documentation_manual__build_from_source.html)
- [ULIS API Reference](classes.html)
- [ULIS Related Pages](pages.html)
- [ULIS Github Repository](https://github.com/Praxinos/ULIS)
- [ULIS EULA](ULIS_EULA.txt)
- [ULIS CLUF](ULIS_CLUF.txt)
- [Praxinos Discord Server](https://discord.com/invite/gEd6pj7)
- [Praxinos Patreon Page](https://www.patreon.com/praxinos)
- [Praxinos Website](https://praxinos.coop/)

## Features
![](meta/image/sample/BlendNormals.png) | ![](meta/image/sample/Unicode.png) | ![](meta/image/sample/Transforms.png) | ![](meta/image/sample/BrownianNoise.png)
---- | ---- | ---- | ----
![](meta/image/sample/BezierDeform.png) | ![](meta/image/sample/Text.png) | ![](meta/image/sample/Raster.png) | ![](meta/image/sample/VoronoiNoise.png) |
![](meta/image/sample/Gradient.png) | ![](meta/image/sample/MipMap.png) | ![](meta/image/sample/RasterText.png) | ![](meta/image/sample/Interactive.png)

This is a non-exhaustive list of important ULIS Features ( see also the [FContext API Reference](class_f_context-members.html) ):
    - Asynchronous operations with multithreading and SIMD optimisations
    - Cross-platform: Windows, macOS, Linux
    - Supported by major compilers: MSVC, Clang/LLVM, GCC, MinGW
    - Many color models ( RGB, Grey, HSV, HSL, Lab, XYZ, YUV )
    - Different depths ( 8bit, 16bit, 32bit, floating point or integer )
    - Many image formats by using any combination of model, depths, and memory layout ordering ( interleaved / planar ( wip ) )
    - Image Clear / Fill / Copy
    - Load / Save from disk in common image formats ( jpeg, png, tiff, hdr )
    - Image blending with standard blend modes and more
    - Text rasterization
    - Shape rasterization
    - MipMap computation
    - Affine transforms, perspective transform, bezier transform
    - Gradients
    - Noise generation
    - Filters with convolution and other advanced operators

## Usage
Here is a small snippet showcasing how to fill a block with black:
```cpp
#include <ULIS>
using namespace ::ULIS;

int
main() {
    // Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );

    // Data
    FBlock block( 1024, 1024, fmt );

    // Operations
    ctx.Fill( block, FColor::Black );
    ctx.Finish();

    return  0;
}
```
