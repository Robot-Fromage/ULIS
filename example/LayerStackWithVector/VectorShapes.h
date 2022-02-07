// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VectorShapes.h
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <blend2d.h>

// https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Basic_Shapes
// We may need to implement more shapes types internally for ease of use
// But the interfaces should stick to the SVG standard.
enum eVectorShape
{
    kRectangle,
    kCircle,
    kEllipse,
    kLine,
    kPolyline,
    kPolygon,
    kPath
};

// https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Fills_and_Strokes
namespace eVectorPaintingAttribute
{
    constexpr uint8_t kNone = 0;
    constexpr uint8_t kStroke = 1;
    constexpr uint8_t kFill = 2;
    constexpr uint8_t kStrokeAndFill = 3;
};

// IVectorShape
class IVectorShape
    : ::ULIS::ITypeIdentifiable
{
public:
    virtual ~IVectorShape() = 0;
    IVectorShape();

public:
    uint8_t VectorPaintingAttribute() const;
    BLCompOp StrokeCompOp() const;
    const BLStyle& StrokeStyle() const;
    const BLStrokeOptions& StrokeOptions() const;
    BLCompOp FillCompOp() const;
    const BLStyle& FillStyle() const;
    BLFillRule FillRule() const;
    const BLMatrix2D& Transform() const;

    void SetVectorPaintingAttribute( uint8_t iVectorPaintingAttribute );
    void SetStrokeCompOp( BLCompOp iCompOp );
    void SetStrokeStyle( BLStyle iStyle );
    void SetStrokeOptions( BLStrokeOptions iOptions );
    void SetFillCompOp( BLCompOp iCompOp );
    void SetFillStyle( BLStyle iStyle );
    void SetFillRule( BLFillRule iRule );
    void SetTransform( const BLMatrix2D& iTransform );

    void ResetAttributes();

    static void SetContextAttributesForStroke( BLContext& iCtx, const IVectorShape& iPrim );
    static void SetContextAttributesForFill( BLContext& iCtx, const IVectorShape& iPrim );

private:
    // Core Rendering Option ( bitflag )
    uint8_t mVectorPaintingAttribute;

    // Stroke Option
    BLCompOp mStrokeCompOp;
    BLStyle mStrokeStyle;
    BLStrokeOptions mStrokeOptions;

    // Fill Option
    BLCompOp mFillCompOp;
    BLStyle mFillStyle;
    BLFillRule mFillRule;

    // Transform
    BLMatrix2D mTransform;
};

// FRectangleVectorShape
class FRectangleVectorShape
    : public IVectorShape
{
public:
    ~FRectangleVectorShape() override;
    FRectangleVectorShape( const BLRect& iData );

public:
    BLRect& Rectangle();
    const BLRect& Rectangle() const;
    void SetRectangle( const BLRect& iData );

    // TypeID Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE_EXT( "Rectangle" )

private:
    BLRect mData;
};

// FCircleVectorShape
class FCircleVectorShape
    : public IVectorShape
{
public:
    ~FCircleVectorShape() override;
    FCircleVectorShape( const BLCircle& iData );

public:
    BLCircle& Circle();
    const BLCircle& Circle() const;
    void SetCircle( const BLCircle& iData );

    // TypeID Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE_EXT( "Circle" )

private:
    BLCircle mData;
};

// FPathVectorShape
class FPathVectorShape
    : public IVectorShape
{
public:
    ~FPathVectorShape() override;
    FPathVectorShape( const BLPath& iData );

public:
    BLPath& Path();
    const BLPath& Path() const;
    void SetPath( const BLPath& iData );

    // TypeID Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE_EXT( "Path" )

private:
    BLPath mData;
};

