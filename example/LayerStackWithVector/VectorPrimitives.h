// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VectorPrimitives.h
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <blend2d.h>

enum eVectorPrimitive
{
    kPath = 0,
};

namespace eVectorPaintingAttribute
{
    constexpr uint8_t kNone = 0;
    constexpr uint8_t kStroke = 1;
    constexpr uint8_t kFill = 2;
    constexpr uint8_t kStrokeAndFill = 3;
};

class IVectorPrimitive
{
public:
    virtual ~IVectorPrimitive() = 0;
    IVectorPrimitive( char iType );

public:
    char Type() const;

    uint8_t VectorPaintingAttribute() const;
    BLCompOp StrokeCompOp() const;
    const BLStyle& StrokeStyle() const;
    const BLStrokeOptions& StrokeOptions() const;
    BLCompOp FillCompOp() const;
    const BLStyle& FillStyle() const;
    BLFillRule FillRule() const;

    void SetVectorPaintingAttribute( uint8_t iVectorPaintingAttribute );
    void SetStrokeCompOp( BLCompOp iCompOp );
    void SetStrokeStyle( BLStyle iStyle );
    void SetStrokeOptions( BLStrokeOptions iOptions );
    void SetFillCompOp( BLCompOp iCompOp );
    void SetFillStyle( BLStyle iStyle );
    void SetFillRule( BLFillRule iRule );

    void ResetAttributes();

    static void SetContextAttributesForStroke( BLContext& iCtx, const IVectorPrimitive& iPrim );
    static void SetContextAttributesForFill( BLContext& iCtx, const IVectorPrimitive& iPrim );

private:
    // Primitive Type Interface
    const char mType;

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
};

