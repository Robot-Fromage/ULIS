// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainEditorWindow.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "SMainEditorWindow.h"
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>

#define IMG_SIZE 512
#define VOL_SIZE 64

SWindow::~SWindow()
{
    delete [] m_bitmap;
    glDeleteTextures( 1, &m_tex_id );
    glDeleteFramebuffers( 1, &m_fbo_id );
    delete m_timer;
}

SWindow::SWindow()
    : QOpenGLWidget()
    , m_tex_id( 0 )
    , m_fbo_id( 0 )
    , m_bitmap( nullptr )
    , m_volume( nullptr )
    , m_timer( nullptr )
    , m_actions{ 0 }
{
    // Init geometry
    setFixedSize( IMG_SIZE, IMG_SIZE );

    // Init timer
    m_timer = new QTimer();
    m_timer->setInterval( 0 );
    QObject::connect( m_timer, SIGNAL( timeout() ), this, SLOT( update() ) );
    m_timer->start();

    // Init bitmap
    m_bitmap = new uint8_t[ IMG_SIZE * IMG_SIZE * 3 ];

    // Init Keys
    m_keys[ kMoveLeft        ] = Qt::Key_Q;
    m_keys[ kMoveRight       ] = Qt::Key_D;
    m_keys[ kMoveForward     ] = Qt::Key_Z;
    m_keys[ kMoveBackward    ] = Qt::Key_S;
    m_keys[ kMoveUp          ] = Qt::Key_Space;
    m_keys[ kMoveDown        ] = Qt::Key_Shift;
    m_keys[ kYawRight        ] = Qt::Key_6;
    m_keys[ kYawLeft         ] = Qt::Key_4;
    m_keys[ kPitchUp         ] = Qt::Key_5;
    m_keys[ kPitchDown       ] = Qt::Key_8;
    m_keys[ kCenterCamera    ] = Qt::Key_0;
}

void
SWindow::initializeGL()
{
    // Init gl
    glewInit();

    // Print version
    printf( "Message from [%s], OpenGL version: %s\n", __FUNCSIG__, glGetString( GL_VERSION ) );

    // Build texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glGenTextures( 1, &m_tex_id );

    // Send texture data
    glBindTexture( GL_TEXTURE_2D, m_tex_id );
    glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGB8, IMG_SIZE, IMG_SIZE );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, IMG_SIZE, IMG_SIZE, GL_RGB, GL_UNSIGNED_BYTE, m_bitmap );

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
SWindow::resizeGL( int w, int h )
{
    // Not handled
    return;
}

void
SWindow::paintGL()
{
    Update();
    Render();

    // Update texture
    glBindTexture( GL_TEXTURE_2D, m_tex_id );
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, IMG_SIZE, IMG_SIZE, GL_RGB, GL_UNSIGNED_BYTE, m_bitmap );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, m_fbo_id );
    glBlitFramebuffer( 0, 0, IMG_SIZE, IMG_SIZE, 0, 0, IMG_SIZE, IMG_SIZE, GL_COLOR_BUFFER_BIT, GL_NEAREST );
    glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 );
    glFinish();
}

void
SWindow::keyPressEvent( QKeyEvent* event )
{
    for( int i = 0; i < kMaxActions; ++i )
        if( event->key() == m_keys[ i ] ) m_actions[ i ] = true;
}

void
SWindow::keyReleaseEvent( QKeyEvent* event )
{
     for( int i = 0; i < kMaxActions; ++i )
        if( event->key() == m_keys[ i ] ) m_actions[ i ] = false;
}

void
SWindow::Update()
{
    /*
    float translation_speed = 5.f;
    float rotation_speed = 0.05f;
    if( m_actions[ kMoveLeft ] )        m_camera.move_right( -translation_speed );
    if( m_actions[ kMoveRight ]  )      m_camera.move_right( translation_speed );
    if( m_actions[ kMoveForward ] )     m_camera.move_forward( translation_speed );
    if( m_actions[ kMoveBackward ] )    m_camera.move_forward( -translation_speed );
    if( m_actions[ kMoveUp ] )          m_camera.move_up( translation_speed );
    if( m_actions[ kMoveDown ] )        m_camera.move_up( -translation_speed );
    if( m_actions[ kYawRight ] )        m_camera.precess( rotation_speed );
    if( m_actions[ kYawLeft ] )         m_camera.precess( -rotation_speed );
    if( m_actions[ kPitchUp ] )         m_camera.nutate( rotation_speed );
    if( m_actions[ kPitchDown ] )       m_camera.nutate( -rotation_speed );
    if( m_actions[ kCenterCamera ] )    m_camera.target( glm::vec3( VOL_SIZE / 2.f ) );
    */
}

void
SWindow::Render()
{
}

