## Formats
So far, ULIS is able to manage approximately 700 different formats.  
Formats layouts are memory-ordered, endianness shouldn't matter as far as ULIS is concerned.  
Planar formats, linear formats, colorspace indicators, and premultiplied formats are still not entirely supported.  
Some properties, such as the model, reverse status, swap status, the premultiplied status or the linear status are allowed to change under some strict circumstances, other properties such as the type, extra alpha indicator or depth ( bytes per sample ) are not allowed to change.
Formats are defined using a 32bits-wide bitfield defined as such:

```cpp
/////////////////////////////////////////////////////
// Format Magic Number, Memory Layout and Properties
//
//        32|        |  20|  16|            8|   4|   0
//          1098 7654 3210 9876     5432 1098 7654 3210
//          XXXX XILP CCCC BBBB     ASRF MMMM NNNN TTTT
//
//    Example: RGBA8
//                            1     1000 0010 0011 0000
//
//          Captions:
//          T: Type                 R: Reverse
//          N: Num Channels         S: Swap
//          M: Model                A: Alpha
//          F: Floating             B: Bytes per sample
//
//          C: Default Profile
//          P: Premultiplied
//          L: Linear ( Gamma Uncompressed )
//          I: Interleaved ( 0: Interleaved, 1: Planar )
//
//          X: Free space for dynamic extra info
//
```

See [this page](_formats_8h.html) for a full list of available formats.

