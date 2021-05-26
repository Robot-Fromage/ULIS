// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <chrono>
using namespace ::ULIS;

class IKLayer
    : public virtual TNode< IKLayer >
{
    typedef TNode< IKLayer > tNode;
    typedef TNode< IKLayer > tParent;
    typedef TNode< IKLayer > tSuperClass;
    typedef IKLayer tSelf;

public:
    virtual ~IKLayer() override = 0
    {}

    IKLayer( const FString& iName = "Untitled", tParent iParent = nullptr )
        : tNode( iParent )
        , mName( iName )
    {}

    const FString& Name() const {
        return  mName;
    }

    void SetName( const FString& iName ) {
        mName = iName;
    }

private:
    FString mName;
};

class FKLayerImage
    : public IKLayer
{
    typedef TNode< IKLayer > tNode;
    typedef TNode< IKLayer > tParent;
    typedef IKLayer tSuperClass;
    typedef FKLayerImage tSelf;

public:
    virtual ~FKLayerImage() override
    {}

    FKLayerImage(
          const FString& iName = "Untitled"
        , eBlendMode iBlendMode = Blend_Normal
        , tParent iParent = nullptr
    )
        : tSuperClass( iName, iParent )
        , mBlendMode( iBlendMode )
    {}

    eBlendMode BlendMode() const {
        return  mBlendMode;
    }

    void SetBlendMode( eBlendMode iBlendMode ) {
        mBlendMode = iBlendMode;
    }

private:
    eBlendMode mBlendMode;
};

class FKLayerFolder final
    : public FKLayerImage
    , public TConcreteRoot< FKLayerFolder, IKLayer >
{
    typedef TNode< IKLayer > tNode;
    typedef TNode< IKLayer > tParent;
    typedef FKLayerImage tSuperClassLayer;
    typedef TConcreteRoot< FKLayerFolder, IKLayer > tSuperClassRoot;
    typedef FKLayerFolder tSelf;

public:
    ~FKLayerFolder() override
    {}

    FKLayerFolder(
          const FString& iName = "Untitled"
        , eBlendMode iBlendMode = Blend_Normal
        , bool iCollapsed = false
        , tParent iParent = nullptr
    )
        : tSuperClassLayer( iName, iBlendMode, iParent )
        , tSuperClassRoot()
        , mCollapsed( iCollapsed )
    {}

    bool Collapsed() const {
        return  mCollapsed;
    }

    void SetCollapsed( bool iCollapsed ) {
        mCollapsed = iCollapsed;
    }

private:
    bool mCollapsed;
};

class FKLayerStack final
    : public TConcreteRoot< FKLayerStack, IKLayer >
{
    typedef TNode< IKLayer > tNode;
    typedef TConcreteRoot< FKLayerStack, IKLayer > tSuperClass;
    typedef FKLayerStack tSelf;

public:
    ~FKLayerStack() override
    {}

    FKLayerStack(
          uint16 iWidth = 1024
        , uint16 iHeight = 1024
        , eFormat iFormat = eFormat::Format_RGBA8
    )
        : tSuperClass()
        , mWidth( iWidth )
        , mHeight( iHeight )
        , mFormat( iFormat )
    {}

    uint16 Width() const {
        return  mWidth;
    }

    uint16 Height() const {
        return  mHeight;
    }

    eFormat Format() const {
        return  mFormat;
    }

private:
    void SetWidth( uint16 iWidth ) {
        mWidth = iWidth;
    }

    void SetHeight( uint16 iHeight ) {
        mHeight = iHeight;
    }

    void SetFormat( eFormat iFormat ) {
        mFormat = iFormat;
    }

private:
    uint16 mWidth;
    uint16 mHeight;
    eFormat mFormat;
};

int
main( int argc, char *argv[] ) {


    FKLayerStack* stack = new FKLayerStack();
    FKLayerFolder* folder = new FKLayerFolder();
    folder->AddChild( new FKLayerImage() ).AddChild( new FKLayerImage() ).AddChild( new FKLayerImage() );
    stack->AddChild( folder );
    stack->AddChild( new FKLayerImage() );
    delete stack;

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_Max );
    FHardwareMetrics hw;
    FSchedulePolicy policy_sync_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_sync_mono_chunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
    FSchedulePolicy policy_sync_multi_scanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );
    FSchedulePolicy policy_sync_mono_scanlines( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Scanlines );

    FFontEngine fontEngine;
    FFont font( fontEngine, "Segoe UI", "Bold" );

    uint16 w = 1024;
    uint16 h = 1024;
    FBlock canvas( w, h, fmt );
    FLayerStack layerStack( w, h, fmt );
    FLayerText* text0 = new FLayerText( "Text0", w, h, fmt, font, Blend_Dissolve, Alpha_Normal, 0.5f, L"TEXT", 16, FMat3F::MakeTranslationMatrix( 50, 50 ), FColor::Black, true, &layerStack );
    FLayerFolder* folder0 = new FLayerFolder( "folder0", w, h, fmt, Blend_Dissolve, Alpha_Normal, 0.9f, &layerStack );
    FLayerImage* image0 = new FLayerImage( "image0", w, h, fmt, Blend_Color, Alpha_Normal, 0.5f, &layerStack );
    FLayerImage* image1 = new FLayerImage( "image1", w, h, fmt, Blend_Normal, Alpha_Normal, 0.5f, &layerStack );
    FLayerImage* image2 = new FLayerImage( "image2", w, h, fmt, Blend_BayerDither8x8, Alpha_Normal, 0.5f, &layerStack );
    FLayerImage* image3 = new FLayerImage( "image3", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerImage* image00 = new FLayerImage( "image00", w, h, fmt, Blend_Normal, Alpha_Erase, 1.f, &layerStack );
    FLayerImage* image01 = new FLayerImage( "image01", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerFolder* folder1 = new FLayerFolder( "folder1", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerImage* image10 = new FLayerImage( "image10", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerImage* image11 = new FLayerImage( "image11", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerImage* image12 = new FLayerImage( "image12", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );
    FLayerImage* image13 = new FLayerImage( "image13", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack );

    //FLayerFolder* folder00 = new FLayerFolder( "folder00", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, folder0 );
    layerStack.AddLayer( text0 );
    layerStack.AddLayer( image0 );
    layerStack.AddLayer( image1 );
        layerStack.AddLayer( folder0 );
            folder0->AddLayer( image00 );
    //folder0->AddLayer( folder00 );
    //folder00->AddLayer( new FLayerImage( "image000", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    //folder00->AddLayer( new FLayerImage( "image001", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
    //folder00->AddLayer( new FLayerImage( "image002", w, h, fmt, Blend_Normal, Alpha_Normal, 1.f, &layerStack ) );
            folder0->AddLayer( image01 );
        layerStack.AddLayer( folder1 );
            folder1->AddLayer( image10 );
            folder1->AddLayer( image11 );
            folder1->AddLayer( image12 );
            folder1->AddLayer( image13 );
    layerStack.AddLayer( image2 );
    layerStack.AddLayer( image3 );

    {
        ctx.Clear( canvas, canvas.Rect(), policy_sync_cache_efficient );

        FEvent eventClear0;
        FEvent eventFill0;
        ctx.Clear( image0->Block(), image0->Block().Rect(), policy_sync_cache_efficient, 0, nullptr, &eventClear0 );
        ctx.Fill( image0->Block(), FColor::RGBA8( 0, 255, 255, 100 ), FRectI( 10, 50, 504, 1004 ), policy_sync_cache_efficient, 1, &eventClear0, &eventFill0 );
        ctx.DrawCircleAA( image0->Block(), image0->Block().Rect().Size() / 2, 128, FColor::RGB( 255, 0, 255 ), true, image0->Block().Rect(), FSchedulePolicy(), 1, &eventFill0, nullptr );

        FEvent eventClear1;
        ctx.Clear( image1->Block(), image1->Block().Rect(), policy_sync_cache_efficient, 0, nullptr, &eventClear1 );
        ctx.Fill( image1->Block(), FColor::RGB( 0, 255, 0 ), FRectI( 500, 500, 800, 800 ), policy_sync_cache_efficient, 1, &eventClear1 );

        FEvent eventClear2;
        ctx.Clear( image2->Block(), image2->Block().Rect(), policy_sync_cache_efficient, 0, nullptr, &eventClear2 );
        ctx.Fill( image2->Block(), FColor::RGB( 255, 0, 0 ), FRectI( 20, 20, 100, 200 ), policy_sync_cache_efficient, 1, &eventClear2 );

        FEvent eventClear00;
        ctx.Clear( image00->Block(), image00->Block().Rect(), policy_sync_cache_efficient, 0, nullptr, &eventClear00 );
        ctx.Fill( image00->Block(), FColor::RGB( 0, 0, 0 ), FRectI( 410, 410, 380, 380 ), policy_sync_cache_efficient, 1, &eventClear00 );

        FEvent eventClear01;
        ctx.Clear( image01->Block(), image01->Block().Rect(), policy_sync_cache_efficient, 0, nullptr, &eventClear01 );
        ctx.Fill( image01->Block(), FColor::RGB( 255, 255, 0 ), FRectI( 400, 400, 400, 400 ), policy_sync_cache_efficient, 1, &eventClear01 );
        ctx.Fill( image3->Block(), FColor::RGBA8( 0, 0, 0, 100 ), image3->Block().Rect(), policy_sync_cache_efficient );
        ctx.Finish();
    }

    auto startTime = std::chrono::steady_clock::now();
    {
        ctx.Flatten(
              layerStack
            , canvas
            , canvas.Rect()
            , FVec2I( 0 )
            , policy_sync_multi_scanlines
            , 0
            , nullptr
            , nullptr
        );
        ctx.Finish();
    }

    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();

    std::cout << "ULIS Layer Stack Flatten took: " << delta << "ms." << std::endl;

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas.Bits()
                                        , canvas.Width()
                                        , canvas.Height()
                                        , canvas.BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    // Get error code. This function returns when all Qt Windows are closed.
    int exit_code = app.exec();

    // Delete Qt Components.
    delete  label;
    delete  image;
    delete  widget;

    // Return exit code.
    return  exit_code;
}

