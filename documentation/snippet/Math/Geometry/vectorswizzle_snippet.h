//! [SwizzleSample]
    FVec2F vec2( 5.0f, 0.75f );
    FVec3F swizzledVec3xxx = vec2.xxx();    // ( 5.0f, 5.0f, 5.0f )
    FVec4F swizzledVec4yxxy = vec2.yxxy();  // ( 0.75f, 5.0f, 5.0f, 0.75f )

    FVec4I vec4( 0, 1, 2, 3 );
    FVec4I swizzledVec4zzzw = vec4.zzzw();  // ( 2, 2, 2, 3 )
//! [SwizzleSample]
