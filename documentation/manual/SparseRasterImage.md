## Sparse Raster Image

### Concept
ULIS has a feature called Sparse Raster Images.  
Sparse Raster Images are sparse quadtrees that hold a graph structure, which leaves are ultimately "chunks" or "tiles", a regular contiguous buffers of raster image data ( bitmaps ).  
The Sparse Raster Images can be applied in two different contexts, that may overlap together:
- Providing a pseudo infinite image canvas size with efficient memory usage and optimized edition features.
- Providing a pseudo infinite image sequence length with efficient memory usage and optimized edition features.

Using this feature, it is not unreasonable to imagine editing an image of size `300 000 * 300 000 px²`, or an animated sequence of hundreds or thousands of frames, with efficient usage of RAM and disk storage, involving some streaming and losless compression steps. This kind of application would be impossible and would bloat the memory if using an approach of allocating such huge contiguous buffers or sequences directly and naively.

### Details
Sparse Raster Images, are also sometime called Tiled Blocks, Tiled Images or Infinite Images, but the appropriate denomination would be Sparse Pixel Quadtree Raster Images.  
This is a theme most often involved in representing 3D Volumetric voxel raster data, such as in Sparse Voxel Octrees, because of the nature of volumetric data and its sheer volume in terms of data size, but applied to extreme cases in the field of 2D graphics.  
Implementing such structures is not as common as other widespread fields of computer imaging, and most of the literature on the subject is targeted at 3D applications( [link](https://research.nvidia.com/sites/default/files/pubs/2010-02_Efficient-Sparse-Voxel/laine2010tr1_paper.pdf) ), but the problem is mostly similar.  
In ULIS, Sparse Raster Images follow this simple hierarchy:
- A macro size is chosen, this macro size is a power of two and describe the maximum size covered by one Quadtree entry.
- A micro size is chosen, this micro size is a power of two and describe the minimum size covered by one Tile entry, that is a leaf element of the subdivised Quadtree graph.
- A map or unordered map is used to store the macro Quadtree entries, the hash of the entry is obtained by mangling the bits of the Quadtree entry spatial location over a bitfield that is either 32-bits or 64-bits wide.
- A root Quadtree entry, with the size of the macro size, is created when visual data is written in a given spatial location where there was initially no information.
- The root Quadtree entry is broken into subdivisions recursively until reaching the leaf depth, where a tile is created, with the size of the micro size.

A common recommend setup for chosing micro and macro coefficients is dictated by hardware, using a macro size of 1024, and a micro size of 64. That is because CPU algorithms work well with power of twos, GPU hardware works well with texture with a power of two which accelerates the computing of mip-maps and live streaming tiles for display. Also, chosing a micro tile size of 64 is efficient for working with 8-bit depth images since a whole tile can fit in a cache line.

A Sparse Raster Image initially has no size and no location on its own, but it has an implicit orthonormal coordinate system. Because of that, data can be written anywhere, at negative coordinates or at insanely large coordinates. Data can be written at sparse non-contiguous locations, and the void beetween different chunks of visual data has a minimal to no impact at all on the memory usage. Furthermore, a background task continuously sanitizes the Quadtrees, checks for empty entries and optimizes away fully transparent or fully filled leaves entries, actually merging them together, so that redundant data is only stored once. This also applies to different Sparse Raster Images sharing the same resource pool, the same data used in multiple occurences over time is merged together to save memory.

Additionally, data can be marked automatically with different labels to indicate to the Sparse Raster Image if some portion can be fully uncompressed on RAM ( high priority areas ), RLE-compressed on RAM ( medium priority areas ), or RLE-compressed and dumped on disk to save memory ( low priority areas ). High, Medium and Low priority areas can be labelled based on the spatial location of the parts of the image, or based on their position in time within a sequence of images. In that regard, wether we extend the image in space or in time is actually very similar.

Because of the shapeshifting nature and the sparsity of the geometry of a Sparse Raster Image, keeping track of the actual geometry of visual data at a given point in time can be challenging. Because of that, ULIS provides four levels of precision to keep track of geometry, depending on the level of precision needed for a given operation. The most precision needed, the most intense the geometry calculation is. However, once computed, if no change occured, the geometry can be cached for later use and is very lightweight in and by itself:
- Operative Geometry
- Rough Root Geometry
- Rough Leaf Geometry
- Exact Leaf Geometry

Operative geometry is the geometry computed from editing operations applied to the Sparse Raster Image, wether it's erasing a part of the image, drawing a stroke or filling an area. It can only grow and often represents the most precise geometry, but can fall out of sync when erasing parts of the data. It is fast to compute because it is incremental.

Rough Root Geometry is the geometry computed from the bounding boxes of the existing root Quadtree entries after sanitization, they represent a rough estimation of the spatial domain covered by the visual data, and even if imprecise, because it is rounded to the closest macro size ( e.g: rounded to the nearest multiple of 1024 ), it is not out of sync with erased parts of the data, and effectively eliminates most of the empty space in the image. It is very fast to compute, because it simply needs to iterate over a few root entries.

Rough Leaf Geometry is the geometry computed from the bounding boxes of the existing leaf entries after sanitization, they represent a finer grained estimation of the spatial domain covered by the visual data, although it does have some imprecision because it is rounded to the closest micro size ( e.g: rounder to the nearest multiple of 64 ), it is synchronized with the actual visual data and efficiently discriminates part of the image with important features, and parts of the image with empty space; It is a bit more heavy to compute but it can be done in the background iteratively.

Exact Geometry is the geometry computed from analyzing the pixel data within the leaves and tiles at the frontier or the Rough Leaf Geometry. It is less efficient but provides the exact bouding box of the data spatial coverage down to the pixel. This is a geometry that is generally not needed for runtime operations, unless if dumping the Sparse Raster Image into regular Contiguous Raster Image ( see FRasterImage2D ) for further processing or export.

