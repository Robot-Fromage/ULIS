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
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#undef RGB
#include <ULIS>

#define IMG_SIZEX 800
#define IMG_SIZEY 600

SViewport::~SViewport()
{
    delete [] m_bitmap;
    glDeleteTextures( 1, &m_tex_id );
    glDeleteFramebuffers( 1, &m_fbo_id );
    delete m_timer;
}

SViewport::SViewport( QWidget* iParent )
    : QOpenGLWidget( iParent )
    , m_tex_id( 0 )
    , m_fbo_id( 0 )
    , m_bitmap( nullptr )
    , m_timer( nullptr )
{
    // Init timer
    m_timer = new QTimer();
    m_timer->setInterval( 0 );
    QObject::connect( m_timer, SIGNAL( timeout() ), this, SLOT( update() ) );
    m_timer->start();

    // Init bitmap
    m_bitmap = new uint8_t[ IMG_SIZEX * IMG_SIZEY * 3 ];
    for( int i = 0; i < IMG_SIZEX * IMG_SIZEY * 3; ++i )
        m_bitmap[i] = ((i/3)%2) * 50 + 20;
}

void
SViewport::initializeGL()
{
    // Init gl
    glewInit();

    // Print version
    printf( "[cpp]> [%s], OpenGL version: %s\n", __FUNCSIG__, glGetString( GL_VERSION ) );

    // Build texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glGenTextures( 1, &m_tex_id );

    // Send texture data
    glBindTexture( GL_TEXTURE_2D, m_tex_id );
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGB8, IMG_SIZEX, IMG_SIZEY );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, IMG_SIZEX, IMG_SIZEY, GL_RGB, GL_UNSIGNED_BYTE, m_bitmap );

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

    // Update texture
    //glBindTexture( GL_TEXTURE_2D, m_tex_id );
    //glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, IMG_SIZEX, IMG_SIZEY, GL_RGB, GL_UNSIGNED_BYTE, m_bitmap );

    int dstw = ::ULIS::FMath::Min( width(), IMG_SIZEX );
    float imageRatio = float( IMG_SIZEX ) / float( IMG_SIZEY );
    int dsth = dstw / imageRatio;
    int dstx = width() / 2 - dstw / 2;
    int dsty = height() / 2 - dsth / 2;
    dstw += dstx;
    dsth += dsty;
    glClearColor( 0.04f, 0.04f, 0.04f, 1 );
    glClear( GL_COLOR_BUFFER_BIT );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fbo_id );
    glBlitFramebuffer(
          0, 0, IMG_SIZEX, IMG_SIZEY
        , dstx, dsty, dstw, dsth
        , GL_COLOR_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
    glFinish();
}

void
SViewport::keyPressEvent( QKeyEvent* event )
{
}

void
SViewport::keyReleaseEvent( QKeyEvent* event )
{
}

void
SViewport::Update()
{
}

void
SViewport::Render()
{
}

