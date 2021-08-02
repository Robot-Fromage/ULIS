//! [Usage]
    // Red color in format RGBA8:
    FColor rgba_color( eFormat::Format_RGBA8, { 255, 0, 0, 255 } );
    uint8 red8 = rgb_color.R8(); // Ok, legal
    float redF = rgb_color.RF(); // No, illegal: doesn't match the format type.

    // Blue color in format BGR8:
    FColor bgr_color( eFormat::Format_BGR8, { 0, 0, 255 } );
    uint8 blue8  = bgr_color.B8();   // Ok, cool.
    uint8 alpha8 = bgr_color.A8();   // Ok, safe to use even if the format doesn't have alpha, value defaults to type maximum.
    uint8 key8   = bgr_color.Key8(); // No, illegal: access via a model that has more channels lead to out of bounds access.
    uint8 hue8   = bgr_color.Hue8(); // Ok, legal: access through model with same channel count, but return value is meaningless.
    uint8 lightF = bgr_color.LightnessF(); // No, illegal: bad type, and meaningless resulting value in non matching models.
//! [Usage]
