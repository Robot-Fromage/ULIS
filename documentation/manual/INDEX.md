## ULIS Documentation
Welcome to the official ULIS documentation.  
ULIS is a rendering library written in C++. It is aimed at software rasterization and digital image processing. ULIS stands for  **U**tility **L**ibrary for **I**maging **S**ystems. ULIS is still a young project, development is ongoing, but it is already powerful enough to compete with other image processing libraries. The goal of ULIS is to provide a single dependency for applications with interactive drawing capabilities and performance in mind. It features a lot of classical image processing algorithms, as well as an advanced asynchronous programming model. All features are designed to take advantage of parallelism on multicore systems, and use the latest available SIMD instructions sets whenever available ( AVX2, SSE4.2, ... ). ULIS also strives to stay a small dependency with small binary size, that is easy to build and integrate into other projects with CMake. To accomodate the needs for a high performance library, ease of use of the application interface, output binary size, and compilation time, the library uses an internal dispatch mechanism: features are compiled for a generic case, then, if possible, using SIMD extensions, and then using format specializations

You can find some tutorials in the [Related Pages](pages.html) section.  
You can find some extensive samples in the sample/ directory for all major ULIS features.  
For the API reference, we advise to start browsing the [Class Index](classes.html) section.  
Have fun using ULIS.

## Build
Follow [this tutorial](md_documentation_manual__build_from_source.html) to build ULIS.

## Licensing
The ULIS repository and all related materials are licensed under a proprietary license.
See the **EULA** or **CLUF** at the root of the repository, or contact Praxinos for more informations.

Alternatively the **EULA** and **CLUF** are available here:
+ [ULIS EULA](ULIS_EULA.txt)
+ [ULIS CLUF](ULIS_CLUF.txt)

## Links
- [Official Github Repository](https://github.com/Praxinos/ULIS)  
- [Praxinos Website](https://praxinos.coop/)  

