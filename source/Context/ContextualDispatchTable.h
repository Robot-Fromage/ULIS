// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ContextualDispatchTable.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FContextualDispatchTable class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Context/Context.h"
#include "Scheduling/Command.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FContext::FContextualDispatchTable
struct FContext::FContextualDispatchTable
{
    friend class FContext;

public:
    /*! Constructor */
    FContextualDispatchTable( eFormat iFormat, ePerformanceIntent iPerfIntent );

    /*! Destructor */
    ~FContextualDispatchTable();

#ifdef ULIS_FEATURE_BLEND_ENABLED
    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlend( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMisc;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparable;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparable;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlendSubpixel( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMiscSubpixel;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparableSubpixel;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparableSubpixel;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleTiledBlend( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleTiledBlendMisc;
            case BlendQualifier_Separable       : return  mScheduleTiledBlendSeparable;
            case BlendQualifier_NonSeparable    : return  mScheduleTiledBlendNonSeparable;
            default: return  nullptr;
        }
    }
#endif // ULIS_FEATURE_BLEND_ENABLED

#ifdef ULIS_FEATURE_TRANSFORM_ENABLED
    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleResize( eResamplingMethod iResamplingMethod ) const
    {
        switch( iResamplingMethod ) {
            case Resampling_Area            : return  mScheduleResizeArea;
            case Resampling_Bicubic         : return  mScheduleResizeBicubic;
            case Resampling_Bilinear        : return  mScheduleResizeBilinear;
            case Resampling_NearestNeighbour: return  mScheduleResizeNN;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleTransformAffine( eResamplingMethod iResamplingMethod ) const
    {
        switch( iResamplingMethod ) {
            case Resampling_Area            : return  mScheduleTransformAffineBilinear;
            case Resampling_Bicubic         : return  mScheduleTransformAffineBicubic;
            case Resampling_Bilinear        : return  mScheduleTransformAffineBilinear;
            case Resampling_NearestNeighbour: return  mScheduleTransformAffineNN;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleTransformAffineTiled( eResamplingMethod iResamplingMethod ) const
    {
        switch( iResamplingMethod ) {
            case Resampling_Area            : return  mScheduleTransformAffineTiledBilinear;
            case Resampling_Bicubic         : return  mScheduleTransformAffineTiledBicubic;
            case Resampling_Bilinear        : return  mScheduleTransformAffineTiledBilinear;
            case Resampling_NearestNeighbour: return  mScheduleTransformAffineTiledNN;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleTransformBezier( eResamplingMethod iResamplingMethod ) const
    {
        switch( iResamplingMethod ) {
            case Resampling_Area            : return  mScheduleTransformBezierBilinear;
            case Resampling_Bicubic         : return  mScheduleTransformBezierBicubic;
            case Resampling_Bilinear        : return  mScheduleTransformBezierBilinear;
            case Resampling_NearestNeighbour: return  mScheduleTransformBezierNN;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleTransformPerspective( eResamplingMethod iResamplingMethod ) const
    {
        switch( iResamplingMethod ) {
            case Resampling_Area            : return  mScheduleTransformPerspectiveBilinear;
            case Resampling_Bicubic         : return  mScheduleTransformPerspectiveBicubic;
            case Resampling_Bilinear        : return  mScheduleTransformPerspectiveBilinear;
            case Resampling_NearestNeighbour: return  mScheduleTransformPerspectiveNN;
            default: return  nullptr;
        }
    }
#endif // ULIS_FEATURE_TRANSFORM_ENABLED

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleRasterGradient( eGradientType iGradientType ) const
    {
        switch( iGradientType ) {
            case Gradient_Linear:       return  mScheduleRasterGradientLinear;
            case Gradient_Radial:       return  mScheduleRasterGradientRadial;
            case Gradient_Angular:      return  mScheduleRasterGradientAngular;
            case Gradient_Reflected:    return  mScheduleRasterGradientReflected;
            case Gradient_Diamond:      return  mScheduleRasterGradientDiamond;
            default: return  nullptr;
        }
    }

private:
    const eFormat mFormat;
    const ePerformanceIntent mPerfIntent;


#ifdef ULIS_FEATURE_BLEND_ENABLED
    const fpCommandScheduler mScheduleBlendSeparable;
    const fpCommandScheduler mScheduleBlendNonSeparable;
    const fpCommandScheduler mScheduleBlendMisc;
    const fpCommandScheduler mScheduleBlendSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendNonSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendMiscSubpixel;
    const fpCommandScheduler mScheduleAlphaBlend;
    const fpCommandScheduler mScheduleAlphaBlendSubpixel;
    const fpCommandScheduler mScheduleTiledBlendSeparable;
    const fpCommandScheduler mScheduleTiledBlendNonSeparable;
    const fpCommandScheduler mScheduleTiledBlendMisc;
#endif // ULIS_FEATURE_BLEND_ENABLED

#ifdef ULIS_FEATURE_CLEAR_ENABLED
    const fpCommandScheduler mScheduleClear;
#endif // ULIS_FEATURE_CLEAR_ENABLED

#ifdef ULIS_FEATURE_COPY_ENABLED
    const fpCommandScheduler mScheduleCopy;
#endif // ULIS_FEATURE_COPY_ENABLED

#ifdef ULIS_FEATURE_CONV_ENABLED
    const fpCommandScheduler mScheduleConvertFormat;
#endif // ULIS_FEATURE_CONV_ENABLED

#ifdef ULIS_FEATURE_FILL_ENABLED
    const fpCommandScheduler mScheduleFill;
    const fpCommandScheduler mScheduleFillPreserveAlpha;
#endif // ULIS_FEATURE_FILL_ENABLED

#ifdef ULIS_FEATURE_TEXT_ENABLED
    const fpCommandScheduler mScheduleRasterText;
    const fpCommandScheduler mScheduleRasterTextAA;
#endif // ULIS_FEATURE_TEXT_ENABLED

#ifdef ULIS_FEATURE_TRANSFORM_ENABLED
    const fpCommandScheduler mScheduleResizeArea;
    const fpCommandScheduler mScheduleResizeBicubic;
    const fpCommandScheduler mScheduleResizeBilinear;
    const fpCommandScheduler mScheduleResizeNN;
    const fpCommandScheduler mScheduleTransformAffineBicubic;
    const fpCommandScheduler mScheduleTransformAffineBilinear;
    const fpCommandScheduler mScheduleTransformAffineNN;
    const fpCommandScheduler mScheduleTransformAffineTiledBicubic;
    const fpCommandScheduler mScheduleTransformAffineTiledBilinear;
    const fpCommandScheduler mScheduleTransformAffineTiledNN;
    const fpCommandScheduler mScheduleTransformBezierBicubic;
    const fpCommandScheduler mScheduleTransformBezierBilinear;
    const fpCommandScheduler mScheduleTransformBezierNN;
    const fpCommandScheduler mScheduleTransformPerspectiveBicubic;
    const fpCommandScheduler mScheduleTransformPerspectiveBilinear;
    const fpCommandScheduler mScheduleTransformPerspectiveNN;

    const fpCommandScheduler mScheduleProcessBezierDeformField;
#endif // ULIS_FEATURE_TRANSFORM_ENABLED

#ifdef ULIS_FEATURE_IO_ENABLED
    const fpCommandScheduler mScheduleFileLoad;
    const fpCommandScheduler mScheduleFileSave;
#endif // ULIS_FEATURE_IO_ENABLED

    const fpCommandScheduler mScheduleExtract;
    const fpCommandScheduler mScheduleFilter;
    const fpCommandScheduler mScheduleFilterInPlace;
    const fpCommandScheduler mScheduleFilterInto;
    const fpCommandScheduler mSchedulesRGBToLinear;
    const fpCommandScheduler mScheduleLinearTosRGB;
    const fpCommandScheduler mSchedulePremultiply;
    const fpCommandScheduler mScheduleUnpremultiply;
    const fpCommandScheduler mScheduleSanitize;
    const fpCommandScheduler mScheduleSwap;

    const fpCommandScheduler mScheduleBuildSATXPass;
    const fpCommandScheduler mScheduleBuildSATYPass;
    const fpCommandScheduler mScheduleBuildPremultipliedSATXPass;
    const fpCommandScheduler mScheduleBuildPremultipliedSATYPass;

    const fpCommandScheduler mScheduleAnalyzeSmallestVisibleRectXPass;
    const fpCommandScheduler mScheduleAnalyzeSmallestVisibleRectYPass;
    const fpCommandScheduler mScheduleAccumulativeSamplingXPass;
    const fpCommandScheduler mScheduleAccumulativeSamplingYPass;

    const fpCommandScheduler mScheduleConvolve;
    const fpCommandScheduler mScheduleConvolveMax;
    const fpCommandScheduler mScheduleConvolveMin;
    const fpCommandScheduler mScheduleConvolvePremult;
    const fpCommandScheduler mScheduleConvolvePremultMax;
    const fpCommandScheduler mScheduleConvolvePremultMin;
    const fpCommandScheduler mScheduleMorphologicalProcess;
    
    const fpCommandScheduler mScheduleDrawLine;
    const fpCommandScheduler mScheduleDrawLineAA;
    const fpCommandScheduler mScheduleDrawLineSP;
    const fpCommandScheduler mScheduleDrawCircle;
    const fpCommandScheduler mScheduleDrawCircleAA;
    const fpCommandScheduler mScheduleDrawCircleSP;
    const fpCommandScheduler mScheduleDrawArc;
    const fpCommandScheduler mScheduleDrawArcAA;
    const fpCommandScheduler mScheduleDrawArcSP;
    const fpCommandScheduler mScheduleDrawEllipse;
    const fpCommandScheduler mScheduleDrawEllipseAA;
    const fpCommandScheduler mScheduleDrawEllipseSP;
    const fpCommandScheduler mScheduleDrawRotatedEllipse;
    const fpCommandScheduler mScheduleDrawRotatedEllipseAA;
    const fpCommandScheduler mScheduleDrawRotatedEllipseSP;
    const fpCommandScheduler mScheduleDrawRectangle;
    const fpCommandScheduler mScheduleDrawPolygon;
    const fpCommandScheduler mScheduleDrawPolygonAA;
    const fpCommandScheduler mScheduleDrawPolygonSP;
    const fpCommandScheduler mScheduleDrawQuadraticBezier;
    const fpCommandScheduler mScheduleDrawQuadraticBezierAA;
    const fpCommandScheduler mScheduleDrawQuadraticBezierSP;

    const fpCommandScheduler mScheduleRasterGradientLinear;
    const fpCommandScheduler mScheduleRasterGradientRadial;
    const fpCommandScheduler mScheduleRasterGradientAngular;
    const fpCommandScheduler mScheduleRasterGradientReflected;
    const fpCommandScheduler mScheduleRasterGradientDiamond;

    const fpCommandScheduler mScheduleBrownianNoise;
    const fpCommandScheduler mScheduleValueNoise;
    const fpCommandScheduler mScheduleVoronoiNoise;
    const fpCommandScheduler mScheduleWhiteNoise;

#if defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )
    const fpConvertFormat mArgConvForwardBlendNonSeparable;
    const fpConvertFormat mArgConvBackwardBlendNonSeparable;
#endif // defined( ULIS_FEATURE_CONV_ENABLED ) && defined( ULIS_FEATURE_BLEND_ENABLED )

};

ULIS_NAMESPACE_END

