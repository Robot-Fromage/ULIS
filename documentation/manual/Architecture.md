# Architecture
ULIS architecture and design principles, *Clément Berthaud*, 2020

---

## Introduction
The architecture of ULIS does not differ fundamentally from other existing traditional graphic libraries, however it pursues its own philosophy and design principles, which leads to its ease of use and efficiency. We discuss here what choices led to the current design, and try to explain them. Other libraries are mentioned and sometimes compared with, with their respective drawbacks and advantages, assuming the usage context ULIS targets, which is explained in details later.

In ["An Image Processing Library in Modern C++"](https://hal.archives-ouvertes.fr/hal-02176414/document), *Michaël Roynard*, *Edwin Carlinet* and *Thierry Géraud* provide insight to achieve simplicity and efficiency with generic programming, in the context of an image processing library in modern C++. While I don't agree with everything stated here, and although this resource was encountered after the architecture of ULIS4+ was designed, I recommend reading it, since it is relevant to the topic, and it describes common pitfalls ULIS encountered in the early stages of its design. Some principles discussed in the paper will also be discussed here.

---

## Table of Contents
- [0) Overview](@ref r0)
    + [0.0) Goals](@ref r0-0)
    + [0.1) Means](@ref r0-1)
    + [0.2) Audience](@ref r0-2)
    + [0.3) Purpose](@ref r0-3)
    + [0.4) Summary](@ref r0-4)
- [1) Implementation](@ref r1)
    + [1.0) Problems](@ref r1-0)
    + [1.1) Propositions](@ref r1-1)
        - [1.1.0) Raw](@ref r1-1-0)
        - [1.1.1) Template](@ref r1-1-1)
        - [1.1.2) Context](@ref r1-1-2)
    + [1.2) Solution](@ref r1-2)
        - [1.2.0) Concepts](@ref r1-2-0)
        - [1.2.1) Details](@ref r1-2-1)

---

## 0) Overview {#r0}
First, a global view of the range of topics covered by ULIS, its goals and philosophy.

### 0.0) Goals {#r0-0}
ULIS is a rendering library. It is aimed at software rasterization and digital image processing. More precisely, ULIS is specifically aimed at application with heavy image processing in mind, particularly ones that offer interactive image edition capabilities for a wide variety of targets and formats. Wether it is for the animation industry, cinema, VFX, illustration, printing, or any field dealing with images, ULIS provides efficient tools that allow easy ways to get the job done.

### 0.1) Means {#r0-1}
ULIS is written in "modern" C++, as of 2020, ULIS uses the C++14 version of the language specification. At first, ULIS was using C++17 and was planned to use C++20 as soon as possible, but fall back to using C++14. This choice was primarily dictated by the context in which ULIS was first used and to allow a wider support at the time of writing. C was also considered for ULIS, and ULIS may be rewritten for C someday.

ULIS relies on some dependencies, which are themselves C or C++ libraries that focus on related fields of image processing, or subsets of the domain ULIS strives to cover. In that sense ULIS, while also providing its own implementations to cover other subsets of the image processing field, is an aggregator that unifies interfaces using modern language features to make the experience of programming image processing tools easy and smooth ( examples of such dependencies are Little-CMS2, which takes care of the color managed pipelines and color space support, and freetype2 which takes care of the text and font rendering side. ). ULIS drives these components altogether to offer ergonomic tools from the programmer's point of view.

### 0.2) Audience {#r0-2}
Sometimes, words such as the "client" or the "consumer", or the "developer" are used to designate the audience or the authors of the library, they are worth explaining. While this section may strike as an unnecessarilly convoluted explanation, i bielieve it is worth explaining where ULIS lies in relation to its audience, because too many things are assumed to be known by the reader in the world of development, which can throw off programmers with less experience.

While this may seem obvious, it's worth mentioning that ULIS itself is arranged in a wider scheme of vendors or tools providers, and clients or consumers. As such, a chain of actors implicitely drives the nature of the ULIS project. For instance:
- ULIS as a library uses other libraries as tools which it aggregates, with all due respect for the work involved in these libraries, and in compliance with their respective licensing terms.
- Authors of ULIS use tools such as compilers, IDEs, version control systems or build systems to develop, maintain and deliver the product.
- ULIS is delivered as a library for programmers to learn the API and use it as a tool to help them achieve their goals, without worrying too much about the implementation details of image processing algorithms.

Once ULIS is released as a library, programmers not related to the ULIS project, involved in any kind of software project, wether they are hobbyist, students, professionals, and wether their goal is an open source, free software or commercial software, might decide to use the ULIS library to achieve their goals. These people are software developers with some amount of experience using programming languages such as C++, or other languages covered by the bindings of ULIS, such as Python. They are directly impacted by the design choices of the library, its API, its ABI, and its overall stability. They are the ones who use the tools ULIS provides, and they are reffered to as the downstream, the developers, the programmers, or the clients of the library. They might also use third party tools such as version control systems, and compilers, amongst the ones ULIS officially supports and tests on a regular basis.

Clients of the ULIS library use functions, tools, classes and other components delivered with the library to ease the development process of their own software, until they release the software to their own clients, redistributing parts of ULIS along the way. The software developped this way might be a painting application, or a scientific application manipulating images, or any kind of tool related to image processing, which targets its own audience. From the point of view of ULIS, this is very relevant and an important concern, as the range of platforms that ULIS supports directly impacts which platforms may or may not use ULIS as a library for a software project. This "final" target audience is called the users, the end-users, or the consumers of the library. They run applications on their device, which rely on compiled ULIS binaries to handle the heavy weightlifting when it comes to image processing.

### 0.3) Purpose {#r0-3}
ULIS was born to fill a gap in the available technologies.
Amongst other libraries and solutions that exist, some of them can achieve in parts the same goals as ULIS. This is a non-exhaustive list:
- [Blend2D](https://github.com/blend2d/blend2d)
- [AGG](https://antigrain.com/)
- [OpenCV](https://opencv.org/)
- [ImageMagick](https://imagemagick.org/index.php)
- [GraphicsMagic](http://www.graphicsmagick.org/)
- [Cairo](https://www.cairographics.org/)
- [Skia](https://skia.org/)
- [GIL](https://www.boost.org/doc/libs/1_68_0/libs/gil/doc/html/index.html)
- [Qt](https://www.qt.io/)
- [CImg](https://cimg.eu/)
- [OpenCL](https://www.khronos.org/opencl/)
- [OpenGL](https://www.khronos.org/opengl/)
- [SFML](https://www.sfml-dev.org/)
- [SDL](https://www.libsdl.org/)
- [Little-CMS](http://www.littlecms.com/)
- [Freetype](https://www.freetype.org/)

All of these libraries are suitable for subsets of what ULIS strives to cover. Some of them are focused at rasterizing vector graphics on the CPU, some of them are more general purpose. Some of them are targeted at being used as software rasterizers, while some of them only focus on specific topics such as text or color. Some of them target the workload on CPU, some of them do that on GPU. None of them cover the appropriate scope ULIS tries to encapsulate in an unified fashion just by themselves. Some of them also cover unrelated topics with additional overhead. Overall, one would have to pick several of these libraries, learn their often very different APIs, deal with their own limitations, and ensure inter-operability beetween these components to achieve a solution that would cover the same goals ULIS does.

Amongst these limitations, we find:
- Incomplete support for a wide variety of image formats
- Not enough features available
- Poor or no support for image IO
- Poor or no support for video
- Not generic enough
- Not optimized or efficient enough
- Too much abstraction is left for the user to handle

For instance, none of these libraries provide a very wide variety of image formats, Blend2D, Qt, Cairo or Skia are limited in the choice of image formats to common formats in a software rasterization context, but that may be unsuitable in the image edition world. OpenCV or CImg provide no such limitations, but their level of abstraction is so great that the friction with the application becomes unmanageable, since there are no way to actually identify a format without relying on custom additions over the tools. Actually, there is some level of assumption too, which can be a deal breaker, since CImg doesn't provide tools for interleaved images, while OpenCV doesn't provide tools for planar images right out of the box. OpenCV, Blend2D, AGG and Qt for instance have no built-in concept of a colorspace, which lead to additional work by the client. Little-CMS and Freetype do their job perfectly, but they need to be driven in order to achieve their goal, and their APIs, while great, are both different and the inter-operability is a source of bugs and friction which slows down the developement of an application, not to mention the overhead of maintaining multiple dependencies.

In that sense, the purpose of ULIS is to fill-in the gaps, provide a semantic way to talk about colors, pixels, images, drawing operations, compositing operations, transformations. While not making assumptions about what is common or uncommon in a given context, ULIS can cover an unlimited range of image formats, wether planar or interleaved. It has meaningful ways to associate an image with a color space, or to speak about the order of the elements in the memory layout, while providing minimal friction for the client, and of course minimal overhead to preserve performances at runtime; but most of all the client shouldn't feel the need to go and seek another library solution because in terms of features, ULIS covers everything you want to do in the image processing field, by aggregating all these features in an extensible manner.

Of course we are making a number of bold claims here, and the implementation has to keep up with that ambition, so i'd like to make it clear that while ULIS is trying to solve these problems, it is probably not perfect as a universal library for imaging and i'm sure you can find the same kind of issues the other libraries display on some parts it tries to cover. So it is good to keep in mind that ULIS is still an evolving product. Also, i am myself an avid user of several of these libraries, and was really inspired by some of the authors of these libraries, such as Petr Kobalicek who wrote the Blend2D library, or Maxim Shemanarev who wrote the AGG library.

To account for the imperfect nature of ULIS, some low level elements are made available through dedicated functions, such as retrieving the raw pointer to a memory buffer containing an image, in order to allow inter-operability with any other framework you choose to use in combination to overcome these issues. Of course that defeats the purpose of ULIS in the first place if you have to use a combination of imaging libraries, but sometimes it's also handy to operate with the GPU or pass data to a GUI library for example.

### 0.4) Summary {#r0-4}
> ULIS is a rendering library written in modern C++ with available bindings for other languages such as Python,  
> It is aimed at applications offering interactive image or video edition capabilities, for the industries of Animation, Films, VFX, Illustration, Printing, etc.  
> ULIS aggregates other libraries to handle text or color managed pipelines.  
> It appeals to software developpers willing to smooth the process of developing of an image processing application, and tries to deliver professional quality components.  
> ULIS was born to fill what feels like a gap in the other available imaging libraries, but is also able to inter-operate and work hand to hand with them.

---

## 1) Implementation {#r1}
A review over the implementation principles, guidelines and tools involved in ULIS.

### 1.0) Problems {#r1-0}
Let's see in details what ULIS is trying to solve, why these problems are hard to solve, and examinate the chosen design. The essence of the problems is manyfold: ULIS aims to be usable as an image library, that:
- covers a wide range of features
- is highly efficient and competitive
- offers generic tools and little constraint
- offers an API that feels smooth and easy to learn

Very often, these concepts work against one another, or privileging a concept over another one brings unbalanced architectures which will not satisfy all requirements. For example, offering competitive performance might come at the cost of a more complex API in order to fine tune the performances intents based on the context in the first place. Performing caching of redundant calculations is sometimes usefull, but might prove inefficient if it ends up bloating the memory. Covering a wide range of features can be a hassle because it requires lots of code to maintain. Using generic programming prevents the programmer from making blunt assumptions, and the genericity introduces some amount of overhead where shortcuts could be used. Generic programming makes it difficult to introduce performance improvments using SIMD extension, as the level of abstraction makes the task absurdely difficult and error prone.

Another aspect of these problems, is wether the mechanisms involved to solve them involve run-time decision making, or compile-time decision making. This is not trivial, and to some extent both cases are often needed, as run-time decision making introduces overhead at runtime, and compile-time decision making might slow down compilation time or simply output huge binaries size, which is unacceptable.

A way to alleviate these issues is to consider the following ideas at the core of the designing process:
- Performance is driven by context, so the architecture design should allow the programmer to focus its decision making on the context, not the detail of the implementation.
- Decision making is driven by the set of available concepts, ULIS offers such as classes involved to solve image related problems. These should be simple and few.
- Segmentation of the functional properties of these objects should be clear and unambiguous.
- More features require more code, there is no way around that and it will take effort. However standardizing the segmentation of the underlying implementations should allow better code reuse, browsing, and overall maintenance.

The core idea here is that ULIS should provide well thought concepts to make context the core and emphasis of every situation where ULIS is used. Various concepts propositions are presented, and it should be noted that over the course of its evolution, different solutions were adopted by ULIS.

### 1.1) Propositions {#r1-1}

#### 1.1.0) Raw {#r1-1-0}

#### 1.1.1) Template {#r1-1-1}

#### 1.1.2) Context {#r1-1-2}

### 1.2) Solution {#r1-2}
We discuss the implementation principles of ULIS, to give a better idea of the concepts introduced and involved in building an image processing pipeline that meets the goals set for ULIS.

#### 1.2.0) Concepts {#r1-2-0}

#### 1.2.1) Details {#r1-2-1}

