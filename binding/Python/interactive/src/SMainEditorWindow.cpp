/*
*   kaze
*__________________
* @file         SWindow.cpp
* @author       Clement Berthaud
* @brief        SWindow class definition.
* @copyright    Copyright 2021 Clement Berthaud.
* @license      Please refer to LICENSE.md
*/
#include "SWindow.h"
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>

#define IMG_SIZE 512
#define VOL_SIZE 64

namespace detail {
uint8_t encodeRGB442( uint8_t iR, uint8_t iG, uint8_t iB ) {
    uint8_t r = ( ( iR * 7 ) / 255 ) & 0x7;
    uint8_t g = ( ( ( iG * 7 ) / 255 ) & 0x7 ) << 3;
    uint8_t b = ( ( ( iB * 3 ) / 255 ) & 0x3 ) << 6;
    return  r | g | b;
}

void decodeRGB442( uint8_t iRGB442, uint8_t* oR, uint8_t* oG, uint8_t* oB ) {
    *oR = ( iRGB442 & 0x7 ) * 255 / 7;
    *oG = ( ( iRGB442 >> 3 ) & 0x7 ) * 255 / 7;
    *oB = ( ( iRGB442 >> 6 ) & 0x3 ) * 255 / 3;
}

void plot( uint8_t* iBuf, int iBps, int iX, int iY, uint8_t iR, uint8_t iG, uint8_t iB ) {
    int index = iY * iBps + iX * 3;
    iBuf[index + 0] = iR;
    iBuf[index + 1] = iG;
    iBuf[index + 2] = iB;
}

void plot( uint8_t* iBuf, int iBps, int Bpp, int iX, int iY, int iZ, uint8_t iMat ) {
    int index = iZ * Bpp + iY * iBps + iX;
    iBuf[index] = iMat;
}

uint8_t read( uint8_t* iBuf, int iBps, int Bpp, int iX, int iY, int iZ ) {
    int index = iZ * Bpp + iY * iBps + iX;
    return  iBuf[index];
}

const static uint8_t static_cube_colors[] = {
      encodeRGB442( 255, 0, 0 )
    , encodeRGB442( 0, 255, 0 )
    , encodeRGB442( 0, 0, 255 )
    , encodeRGB442( 255, 255, 0 )
    , encodeRGB442( 0, 255, 255 )
    , encodeRGB442( 255, 0, 255 )
};

} // namespace detail

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
    , m_camera()
    , m_box( glm::vec3( 0.f ), glm::vec3( VOL_SIZE ) )
    , m_actions{ 0 }
{
    // Init geometry
    setFixedSize( IMG_SIZE, IMG_SIZE );

    // Init timer
    m_timer = new QTimer();
    m_timer->setInterval( 0 );
    QObject::connect( m_timer, SIGNAL( timeout() ), this, SLOT( update() ) );
    m_timer->start();

    // Init camera
    m_camera.set_viewport( IMG_SIZE, IMG_SIZE );
    m_camera.set_translation( 64, 64, 64 );
    m_camera.target( glm::vec3( 0, 0, 0 ) );
    m_camera.set_fov( kaze::math::deg_to_rad_f( 90.f ) );

    // Init bitmap
    m_bitmap = new uint8_t[ IMG_SIZE * IMG_SIZE * 3 ];

    // Init volume
    m_volume = new uint8_t[ VOL_SIZE * VOL_SIZE * VOL_SIZE ];
    {
        const int size = VOL_SIZE;
        const int bps = size;
        const int bpp = size * size;
        const glm::vec3 center( size / 2 );
        const float rad( size / 2.f - 1 );
        const float radl( size / 2.f - 4 );
        for( int i = 0; i < size; ++i ) {
            for( int j = 0; j < size; ++j ) {
                for( int k = 0; k < size; ++k ) {
                    glm::vec3 pos( i, j, k );
                    if( glm::distance( pos, center ) < rad && glm::distance( pos, center ) >= radl  ) {
                        detail::plot( m_volume, bps, bpp, i, j, k, 1 );
                    } else {
                        detail::plot( m_volume, bps, bpp, i, j, k, 0 );
                    }
                }
            }
        }
    }

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
}

void
SWindow::Render()
{
    // Tracing primary rays
    glm::vec3 pos = m_camera.translation();
    glm::vec3 a, b, c, d;
    m_camera.world_vectors( &a, &b, &c, &d );
    const float invHeight = 1.f / m_camera.width();
    const float invWidth = 1.f / m_camera.height();
    const glm::vec3 leftNormalYDelta  = ( d - a ) * invHeight;
    const glm::vec3 rightNormalYDelta = ( c - b ) * invHeight;
    glm::vec3 leftNormal = a;
    glm::vec3 rightNormal = b;
    for (int y = 0; y < m_camera.height(); ++y) {
        glm::vec3 direction = leftNormal;
        const glm::vec3 normalXDelta = (rightNormal - leftNormal) * invWidth;
        for (int x = 0; x < m_camera.width(); ++x) {
            uint8_t color = TracePrimaryRay( kaze::ray( pos, direction ) );
            uint8_t r,g,b;
            detail::decodeRGB442( color, &r, &g, &b );
            detail::plot( m_bitmap, IMG_SIZE * 3, x, ( m_camera.height() - 1 ) - y, r, g, b );
            direction += normalXDelta;
        }
        leftNormal += leftNormalYDelta;
        rightNormal += rightNormalYDelta;
    }
}

uint8_t
SWindow::TracePrimaryRay( const kaze::ray& iRay )
{
    float t_min, t_max;
    int side;
    if( m_box.intersect( iRay, t_min, t_max, side ) ) {
        if( t_min == t_max || isnan( t_min ) )
            return  0;

        if( Traversal( iRay, t_min, t_max, side ) )
            return  detail::static_cube_colors[ side ];
    }
    return  0;
}

uint8_t
SWindow::Traversal( const kaze::ray& iRay, float iTMin, float iTMax, int& oSide )
{
    glm::vec3 point_min = iRay.point( iTMin );
    glm::vec3 point_max = iRay.point( iTMax );
    if( point_min == point_max )
        return  0;

    float tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ;

    float dx = kaze::math::sign( point_max.x - point_min.x );
    if( dx != 0 )
        tDeltaX = fminf( dx / ( point_max.x - point_min.x ), FLT_MAX );
    else
        tDeltaX = FLT_MAX;

    if( dx > 0 )
        tMaxX = tDeltaX * kaze::math::frac1(point_min.x);
    else
        tMaxX = tDeltaX * kaze::math::frac0( point_min.x );
    int x = point_min.x;

    float dy = kaze::math::sign( point_max.y - point_min.y );
    if( dy != 0 )
        tDeltaY = fminf( dy / ( point_max.y - point_min.y ), FLT_MAX );
    else
        tDeltaY = FLT_MAX;

    if( dy > 0 )
        tMaxY = tDeltaY * kaze::math::frac1(point_min.y);
    else
        tMaxY = tDeltaY * kaze::math::frac0( point_min.y );
    int y = point_min.y;

    float dz = kaze::math::sign( point_max.z - point_min.z );
    if( dz != 0 )
        tDeltaZ = fminf( dz / ( point_max.z - point_min.z ), FLT_MAX );
    else
        tDeltaZ = FLT_MAX;

    if( dz > 0 )
        tMaxZ = tDeltaZ * kaze::math::frac1(point_min.z);
    else
        tMaxZ = tDeltaZ * kaze::math::frac0( point_min.z );

    int z = point_min.z;

    const int size = VOL_SIZE;
    const int bps = size;
    const int bpp = size * size;
    int xout = dx > 0 ? VOL_SIZE : -1;
    int yout = dy > 0 ? VOL_SIZE : -1;
    int zout = dz > 0 ? VOL_SIZE : -1;

    while (true) {
        uint8_t val = detail::read( m_volume, bps, bpp, x, y, z );
        if( val ) {
            return  val;
        }
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                oSide = iRay.sign[0];
                x += dx;
                if( x == xout ) break;
                tMaxX += tDeltaX;
            } else {
                oSide = 4 + iRay.sign[2];
                z += dz;
                if( z == zout ) break;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                oSide = 2 + iRay.sign[1];
                y += dy;
                if( y == yout ) break;
                tMaxY += tDeltaY;
            } else {
                oSide = 4 + iRay.sign[2];
                z += dz;
                if( z == zout ) break;
                tMaxZ += tDeltaZ;
            }
        }
    }

    return  0;
}

