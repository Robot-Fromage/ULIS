/*
*   kaze
*__________________
* @file         SWindow.h
* @author       Clement Berthaud
* @brief        SWindow class declaration.
* @copyright    Copyright 2021 Clement Berthaud.
* @license      Please refer to LICENSE.md
*/
#pragma once
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <QOpenGLWidget>
#undef near
#undef far
#include <kaze>

class SWindow
    : public QOpenGLWidget
{
    Q_OBJECT

enum eAction {
      kMoveLeft
    , kMoveRight
    , kMoveForward
    , kMoveBackward
    , kMoveUp
    , kMoveDown
    , kYawRight
    , kYawLeft
    , kPitchUp
    , kPitchDown
    , kCenterCamera
    , kMaxActions
};

public:
    ~SWindow() override;
    SWindow();

protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
    virtual void keyPressEvent( QKeyEvent* event ) override;
    virtual void keyReleaseEvent( QKeyEvent* event ) override;

private:
    void Update();
    void Render();
    uint8_t TracePrimaryRay( const kaze::ray& iRay );
    uint8_t Traversal( const kaze::ray& iRay, float iTMin, float iTMax, int& oSide );

private:
    GLuint m_tex_id;
    GLuint m_fbo_id;
    uint8_t* m_bitmap;
    uint8_t* m_volume;
    QTimer* m_timer;
    kaze::camera m_camera;
    kaze::aabb_3d m_box;
    bool m_actions[kMaxActions];
    Qt::Key m_keys[kMaxActions];
};

