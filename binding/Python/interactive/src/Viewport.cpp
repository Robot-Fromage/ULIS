// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Viewport.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Viewport.h"
#include "Code.h"
#include "Console.h"
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <ULIS>

#define IMG_SIZEX 800
#define IMG_SIZEY 600

// Here's how it works: an instance of the output redirection wrapper is
// maintained within the viewport and exported to python by the embeded
// redirect module.
// The instance is shared, and built on the CPP side.
// It only has a print method to forward stdout and stderr to the UI console,
// so it needs to have a weak ptr to the console too.
// Then a small code is executed in __main__ to add the redirector and setup the
// redirection from within python ( with sys.stdout ).
class FOutputRedirectionWrapper
{
public:
    FOutputRedirectionWrapper()
        : mConsole( nullptr )
    {}

    FOutputRedirectionWrapper( SConsole* iConsole )
        : mConsole( iConsole )
    {}

    void
    write( const std::string& iStr ) {
        if( iStr == "\n" )
            return; //mConsole->LogPy( "" );
        else
            mConsole->LogPy( QString( iStr.c_str() ) );
    }

    void
    flush() {
    }

private:
    SConsole* mConsole;
};

PYBIND11_EMBEDDED_MODULE( pyRedirect, m ) {
    py::class_< FOutputRedirectionWrapper >( m, "FOutputRedirectionWrapper" )
        .def( py::init<>() )
        .def( "write", &FOutputRedirectionWrapper::write )
        .def( "flush", &FOutputRedirectionWrapper::flush );
}

SViewport::~SViewport()
{
    py::finalize_interpreter();
    delete mBitmap;
    glDeleteTextures( 1, &m_tex_id );
    glDeleteFramebuffers( 1, &m_fbo_id );
    delete m_timer;
    delete mRedirect;
}

SViewport::SViewport( QWidget* iParent, SCode* iCode, SConsole* iConsole )
    : QOpenGLWidget( iParent )
    , m_tex_id( 0 )
    , m_fbo_id( 0 )
    , mBitmap( nullptr )
    , m_timer( nullptr )
    , mCode( iCode )
    , mConsole( iConsole )
    , mRedirect( new FOutputRedirectionWrapper( iConsole ) )
    , mLaunched( false )
{
    // Init timer
    m_timer = new QTimer();
    m_timer->setInterval( 24.0 / 1000.0 );
    QObject::connect( m_timer, SIGNAL( timeout() ), this, SLOT( update() ) );
    m_timer->start();

    // Init bitmap
    mBitmap = new ::ULIS::FBlock( IMG_SIZEX, IMG_SIZEY, ::ULIS::Format_RGBA8 );
    ::ULIS::uint8* data = mBitmap->Bits();
    for( int i = 0; i < mBitmap->BytesTotal(); i+=4 ) {
        data[i + 0] = ( ( i / 3 ) % 2) * 50 + 20;
        data[i + 1] = ( ( i / 3 ) % 2) * 50 + 20;
        data[i + 2] = ( ( i / 3 ) % 2) * 50 + 20;
        data[i + 3] = 255;
    }

    py::initialize_interpreter();
}

void
SViewport::initializeGL()
{
    // Init gl
    glewInit();

    // Print version
    mConsole->LogCPP( "[" + QString( __FUNCSIG__ ) + "]" + QString( "OpenGLVersion: " ) + QString( (const char*)glGetString( GL_VERSION ) ) );

    // Build texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glGenTextures( 1, &m_tex_id );

    // Send texture data
    glBindTexture( GL_TEXTURE_2D, m_tex_id );
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, IMG_SIZEX, IMG_SIZEY );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, IMG_SIZEX, IMG_SIZEY, GL_RGBA, GL_UNSIGNED_BYTE, mBitmap->Bits() );

    // Build framebuffer
    glGenFramebuffers( 1, &m_fbo_id );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fbo_id );

    // Bind texture to framebuffer
    glFramebufferTexture2D( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_id, 0 );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );

    // Unnecessary
    glFinish();
}

void
SViewport::resizeGL( int w, int h )
{
    //glViewport( 0, 0, w, h );
}

void
SViewport::paintGL()
{
    Update();
    Render();
}

void
SViewport::keyPressEvent( QKeyEvent* event )
{
    if( event->key() == Qt::Key_F5 ) {
        py::finalize_interpreter();
        py::initialize_interpreter();

        // Redirection
        try {
            py::module_ main = py::module_::import("__main__");
            py::module_ redirect = py::module_::import("pyRedirect");
            main.add_object( "__redirector__", py::cast( mRedirect ), true );
            py::exec( R"(
                import sys
                sys.stdout = __redirector__
                sys.stderr = __redirector__
            )" );
        } catch( const std::exception& e ) {
            mConsole->LogPy( e.what() );
            mConsole->LogCPP( "Interpreter exception during stream redirection, error in cpp code." );
        }

        // Main Script Evaluation
        QString script = mCode->document()->toPlainText();
        try {
            py::exec( script.toUtf8().constData() );
        } catch( const std::exception& e ) {
            mConsole->LogPy( e.what() );
            mConsole->LogCPP( "Interpreter exception, error in python script." );
        }

        // Run start
        try {
            py::module_ main = py::module_::import("__main__");
            main.attr( "start" )();
            mLaunched = true;
        } catch( const std::exception& e ) {
            mConsole->LogPy( e.what() );
            mConsole->LogCPP( "Interpreter exception, start() function defined in script." );
        }
    }
}

void
SViewport::keyReleaseEvent( QKeyEvent* event )
{
}

void
SViewport::Update()
{
    // Run update
    if( mLaunched ) {
        try {
            py::module_ main = py::module_::import("__main__");
            main.attr( "update" )( 24.0 / 1000.0 );
        } catch( const std::exception& e ) {
            mConsole->LogPy( e.what() );
            mConsole->LogCPP( "Interpreter exception, update( delta ) function defined in script." );
        }
    }
}

void
SViewport::Render()
{
    // Update texture
    

    if( mLaunched ) {
        ::ULIS::FBlock* canvas = nullptr;
        try {
            py::module_ main = py::module_::import("__main__");
            py::object canvas_attr = main.attr( "canvas" );
            canvas = canvas_attr.cast< ::ULIS::FBlock* >();
        } catch( const std::exception& e ) {
            mConsole->LogPy( e.what() );
            mConsole->LogCPP( "Interpreter exception, Render failed to retrieve variable named canvas." );
        }

        if( canvas && canvas->Bits() != mBitmap->Bits() ) {
            // Non owning steal
            mBitmap->LoadFromData( canvas->Bits(), canvas->Width(), canvas->Height(), canvas->Format(), nullptr, ::ULIS::FOnInvalidBlock(), ::ULIS::FOnCleanupData() );
        }

        glBindTexture( GL_TEXTURE_2D, m_tex_id );
        glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, mBitmap->Width(), mBitmap->Height(), GL_RGBA, GL_UNSIGNED_BYTE, mBitmap->Bits() );
    }

    int dstw = ::ULIS::FMath::Min( (int)width(), (int)mBitmap->Width() );
    float imageRatio = float( mBitmap->Width() ) / float( mBitmap->Height() );
    int dsth = ::ULIS::FMath::Min( (int)height(), int( dstw / imageRatio ) );
    dstw = dsth * imageRatio;
    int dstx = width() / 2 - dstw / 2;
    int dsty = height() / 2 - dsth / 2;
    dstw += dstx;
    dsth += dsty;

    glClearColor( 0.04f, 0.04f, 0.04f, 1 );
    glClear( GL_COLOR_BUFFER_BIT );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fbo_id );
    glBlitFramebuffer(
          0, 0, mBitmap->Width(), mBitmap->Height()
        , dstx, dsty, dstw, dsth
        , GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
    glFinish();
}

